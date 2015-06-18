#include <QGraphicsLineItem>
#include <QGLWidget>
#include <QDebug>
#include <QVBoxLayout>
#include "SplineEditorWidget.hpp"
#include "widgets/SplineEditorView.hpp"
#include "widgets/SplineEditorScene.hpp"
#include "widgets/SplineEditorNode.hpp"
#include "widgets/NodeEditorWidget.hpp"
#include "widgets/KnotVectorWidget.hpp"
#include "widgets/SettingsWidget.hpp"
#include "SplineDataModel.hpp"

// NOTE FOR FUTURE: The coordinates in QGraphicsEllipseItem is in 
// *local* coordinates, *not* scene coordinates!!!

SplineEditorWidget::SplineEditorWidget(QWidget * parent, Qt::WindowFlags f) 
    : QWidget(parent, f)
{
    auto h_layout      = new QHBoxLayout();
    auto v_layout_col0 = new QVBoxLayout();
    auto v_layout_col1 = new QVBoxLayout();

    // column one: graphics view framework stuff and knot vector editor
    m_scene = new SplineEditorScene(-1.0, -1.0, 2.0, 2.0);
    m_view = new SplineEditorView(m_scene);
    m_view->setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
    m_view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
    m_view->fitInView(m_scene->sceneRect());
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    v_layout_col0->addWidget(m_view);

    m_knot_vector_editor = new KnotVectorWidget;
    m_knot_vector_editor->setMaximumHeight(200);
    connect(m_knot_vector_editor, SIGNAL(knot_vector_type_changed(KnotVectorType)),    this, SIGNAL(knot_vector_type_changed(KnotVectorType)));
    connect(m_knot_vector_editor, SIGNAL(eval_interval_changed(qreal, qreal)),         this, SIGNAL(eval_interval_changed(qreal, qreal)));
    connect(m_knot_vector_editor, SIGNAL(auto_knot_limits_changed(qreal, qreal)),      this, SIGNAL(auto_knot_limits_changed(qreal, qreal)));
    connect(m_knot_vector_editor, SIGNAL(knot_vector_manually_edited(QVector<qreal>)), this, SIGNAL(knot_vector_manually_edited(QVector<qreal>)));

    v_layout_col0->addWidget(m_knot_vector_editor);

    // column two: node editor and settings widget
    m_node_editor = new NodeEditorWidget;
    m_node_editor->setEnabled(false);
    connect(m_node_editor, &NodeEditorWidget::node_manually_edited, [&](int node_index, QPointF new_pos) {
        m_node_items[node_index]->setPos(new_pos);
        emit node_moved(node_index, new_pos);
    });
    m_node_editor->setMaximumHeight(200);
    m_node_editor->setMaximumWidth(200);

    v_layout_col1->addWidget(m_node_editor);

    m_settings_widget = new SettingsWidget;
    m_settings_widget->setMaximumWidth(200);
    m_settings_widget->setMaximumHeight(200);
    v_layout_col1->addWidget(m_settings_widget);

    h_layout->addLayout(v_layout_col0);
    h_layout->addLayout(v_layout_col1);
    setLayout(h_layout);
    draw_coordinate_axes();
    setup_connections();
}

void SplineEditorWidget::remove_old_items(QVector<QGraphicsItem*>& items) {
    for (QGraphicsItem* item : items) {
        m_scene->removeItem(item);
        delete item;
    }
    items.clear();
}

// For now: only nodes and knot vector. In future: degree and weights.
void SplineEditorWidget::update_from_model(SplineDataModel::const_ptr model) {
    remove_old_items(m_node_items);
    m_node_editor->setEnabled(false);

    auto control_points = model->get_control_points();
    auto num_nodes = control_points.size();

    // update nodes
    QPen node_pen;
    node_pen.setWidth(0);
    for (int node_index = 0; node_index < num_nodes; node_index++) {
        const auto p = control_points[node_index];
        auto node = new SplineEditorNode(p.x(), p.y(), 0.03, node_index);
        node->setPen(node_pen);
        node->setFlag(QGraphicsItem::ItemIsMovable,   true);
        node->setFlag(QGraphicsItem::ItemIsFocusable, true);
        node->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    
        // we want to be notified by this new node when it moves
        connect(node, SIGNAL(itemMoved(int, QPointF)), this, SIGNAL(node_moved(int, QPointF)));

        // also when the user marks it by clicking on it.
        connect(node, SIGNAL(itemSelected(int)), this, SLOT(on_node_selected(int)));

        // also when user presses the delete key on it
        connect(node, SIGNAL(itemDeleteRequest(int)), this, SIGNAL(node_deleted(int)));

        // should some of them be linked?
        if (m_settings_widget->get_join_ends()) {
            const auto degree = model->get_degree();
            if (node_index >= num_nodes - degree) {
                int node_to_mirror =  degree - (num_nodes - node_index);
                qDebug() << "Node " << node_index << " will mirror node " << node_to_mirror;
                auto casted_src_node = dynamic_cast<SplineEditorNode*>(m_node_items[node_to_mirror]);
                connect(casted_src_node, SIGNAL(itemMoved(int, QPointF)), node, SLOT(set_position(int, QPointF)));

                // then it should only be possible to move the nodes that are mirrored
                auto mirrored_pos = casted_src_node->pos();
                emit node_moved(node_index, mirrored_pos);
                node->setVisible(false);
            }
        }

        m_scene->addItem(node);
        m_node_items.append(node);
    }

    // update knot vector
    m_knot_vector_editor->update_knots(model->get_knot_vector());
}

void SplineEditorWidget::update_rendered_spline(const QVector<QPointF>& points) {
    auto num_points = points.size();
    if (num_points < 2) {
        qDebug() << "Too few visualization points. Ignoring";
        return;
    }

    remove_old_items(m_line_items);
    QPen pen;
    pen.setWidth(0);
    for (int i = 0; i < num_points-1; i++) {
        auto line = new QGraphicsLineItem(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y());
        line->setPen(pen);
        m_line_items.append(line);
        m_scene->addItem(line);
    }
}


void SplineEditorWidget::draw_coordinate_axes() {
    auto x_axis = new QGraphicsLineItem(0.0, 0.0, 0.5, 0.0);
    auto y_axis = new QGraphicsLineItem(0.0, 0.0, 0.0, 0.5);
    QPen pen;
    pen.setWidthF(0.002);
    x_axis->setPen(pen);
    y_axis->setPen(pen);
    m_scene->addItem(x_axis);
    m_scene->addItem(y_axis);
}

void SplineEditorWidget::setup_connections() {
    connect(m_view, SIGNAL(userClickedOnEmptySpace(QPointF)), this, SIGNAL(node_added(QPointF)));
}

void SplineEditorWidget::on_node_selected(int node_index) {
    if ((node_index < 0) || (node_index >= m_node_items.size())) {
        throw std::runtime_error("on_node_selected(): Invalid node index");
    }
    m_node_editor->set_index(node_index);
    m_node_editor->set_position(m_node_items[node_index]->scenePos());
    m_node_editor->setEnabled(true);
}
