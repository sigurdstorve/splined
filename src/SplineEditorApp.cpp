#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QStringList>
#include "SplineEditorApp.hpp"
#include "SplineDataModel.hpp"
#include "widgets/SplineEditorWidget.hpp"

SplineEditorApp::SplineEditorApp(QWidget* parent)
    : QMainWindow(parent)
{
    
    m_editor_widget = new SplineEditorWidget;
    setCentralWidget(m_editor_widget);

    const int degree = 3;
    m_spline_model = SplineModelUtils::Create(degree);
    m_editor_widget->update_from_model(m_spline_model);
    m_editor_widget->update_rendered_spline(m_spline_model->render()); 

    setup_connections();
    setup_menus();
}

void SplineEditorApp::setup_connections() {
    connect (m_editor_widget, &SplineEditorWidget::node_added, [&](QPointF pos) {
        m_spline_model->add_node(pos);
        m_editor_widget->update_from_model(m_spline_model);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::node_deleted, [&](int node_index) {
        m_spline_model->delete_node(node_index);
        m_editor_widget->update_from_model(m_spline_model);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::node_moved, [&](int node_index, QPointF new_pos) {
        m_spline_model->move_node(node_index, new_pos);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::knot_vector_type_changed, [&](KnotVectorType type) {
        m_spline_model->set_knot_vector_type(type);
        m_editor_widget->update_from_model(m_spline_model);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::eval_interval_changed, [&](qreal t0, qreal t1) {
        m_spline_model->set_eval_limits(t0, t1);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::auto_knot_limits_changed, [&](qreal t0, qreal t1) {
        m_spline_model->set_autogen_knot_vector_limits(t0, t1);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    });

    connect(m_editor_widget, &SplineEditorWidget::knot_vector_manually_edited, [&](QVector<qreal> new_knots) {
        qDebug() << "New knot vector: " << new_knots;
    });
}

void SplineEditorApp::setup_menus() {
    auto menu = menuBar();
    auto file_menu = new QMenu("File");
    menu->addMenu(file_menu);

    auto new_spline_act = new QAction("New spline", this);
    file_menu->addAction(new_spline_act);
    connect(new_spline_act, SIGNAL(triggered()), this, SLOT(on_new_spline()));

    auto load_spline_act = new QAction("Load spline", this);
    file_menu->addAction(load_spline_act);
    connect(load_spline_act, SIGNAL(triggered()), this, SLOT(on_load_spline()));

    auto save_spline_act = new QAction("Save spline", this);
    file_menu->addAction(save_spline_act);
    connect(save_spline_act, SIGNAL(triggered()), this, SLOT(on_save_spline()));

    auto exit_act = new QAction("Exit", this);
    file_menu->addAction(exit_act);
    connect(exit_act, SIGNAL(triggered()), this, SLOT(close()));
}

void SplineEditorApp::on_save_spline() {
    auto save_filename = QFileDialog::getSaveFileName(this, "Save spline", ".", "*.txt");
    if (save_filename == "") {
        qDebug() << "No filename given. Skipping save";
        return;
    }
    SplineModelUtils::Save(save_filename, m_spline_model);
}

void SplineEditorApp::on_load_spline() {
    auto load_filename = QFileDialog::getOpenFileName(this, "Load spline", ".", "*.txt");
    if (load_filename == "") {
        qDebug() << "No filename given. Skipping load";
        return;
    }
    auto loaded_spline = SplineModelUtils::Load(load_filename);

    if (loaded_spline != nullptr) {
        m_spline_model = loaded_spline;
        m_editor_widget->update_from_model(m_spline_model);
        m_editor_widget->update_rendered_spline(m_spline_model->render());
    } else {
        qDebug() << "Unable to load spline model";
    }
}

void SplineEditorApp::on_new_spline() {
    bool ok;
    auto model_degree = QInputDialog::getInt(this, "Spline degree", "Degree", 3, 0, 10, 1, &ok);
    if (!ok) {
        qDebug() << "Skipping new spline.";
        return;
    }
    m_spline_model = SplineModelUtils::Create(model_degree);
    m_spline_model->set_degree(model_degree);
    m_editor_widget->update_from_model(m_spline_model);
    m_editor_widget->update_rendered_spline(m_spline_model->render());
}
