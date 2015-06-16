#include <QGraphicsLineItem>
#include <QGLWidget>
#include <QDebug>
#include <QVBoxLayout>
#include "SplineEditorWidget.hpp"
#include "widgets/SplineEditorView.hpp"
#include "widgets/SplineEditorScene.hpp"
#include "widgets/SplineEditorNode.hpp"
#include "SplineDataModel.hpp"

// NOTE FOR FUTURE: The coordinates in QGraphicsEllipseItem is in 
// *local* coordinates, *not* scene coordinates!!!

SplineEditorWidget::SplineEditorWidget(QWidget * parent, Qt::WindowFlags f) 
    : QWidget(parent, f)
{
    auto h_layout      = new QHBoxLayout();
    auto v_layout_col0 = new QVBoxLayout();
    auto v_layout_col1 = new QVBoxLayout();

    // initialize graphics view framework stuff   
    m_scene = new SplineEditorScene(-1.0, -1.0, 2.0, 2.0);
    m_view = new SplineEditorView(m_scene);
    m_view->setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
    m_view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
    m_view->fitInView(m_scene->sceneRect());
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    v_layout_col0->addWidget(m_view);

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

// For now: only nodes. In future: knots and degree and weights.
void SplineEditorWidget::update_from_model(SplineDataModel::ptr model) {
    remove_old_items(m_node_items);

    auto control_points = model->get_control_points();
    auto num_nodes = control_points.size();

    QPen node_pen;
    node_pen.setWidth(0);
    for (int node_index = 0; node_index < num_nodes; node_index++) {
        const auto p = control_points[node_index];
        auto node = new SplineEditorNode(p.x(), p.y(), 0.02, node_index);
        node->setPen(node_pen);
        node->setFlag(QGraphicsItem::ItemIsMovable,   true);
        node->setFlag(QGraphicsItem::ItemIsFocusable, true);
        node->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    
        // we want to be notified by this new node
        connect(node, SIGNAL(itemMoved(int, QPointF)), this, SIGNAL(node_moved(int, QPointF)));

        m_scene->addItem(node);
        m_node_items.append(node);
    }
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
    auto x_axis = new QGraphicsLineItem(0.0, 0.0, 0.15, 0.0);
    auto y_axis = new QGraphicsLineItem(0.0, 0.0, 0.0, 0.1);
    QPen pen;
    pen.setWidthF(0.002);
    x_axis->setPen(pen);
    y_axis->setPen(pen);
    m_scene->addItem(x_axis);
    m_scene->addItem(y_axis);
}

void SplineEditorWidget::setup_connections() {
    // we want to know if user has clicked on a node
    // TODO: connect on a Node-by-Node basis and connect on creation
    connect(m_view, &SplineEditorView::userClickedOnNode, [&](int node_id) {
        qDebug() << "User clicked on node " << node_id << ". TODO: Forward it.";
    });

    connect(m_view, &SplineEditorView::userClickedOnEmptySpace, [&](QPointF pos) {
        emit node_added(pos);
    });
}