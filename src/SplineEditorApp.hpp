#include <QWidget>
#include <QMainWindow>
#include "SplineDataModel.hpp"

class SplineEditorWidget;

class SplineEditorApp : public QMainWindow {
Q_OBJECT
public:
    SplineEditorApp(QWidget* parent = 0);

private slots:
    void on_save_spline();

    void on_load_spline();

    void on_new_spline();

private:

    // Setup connections so that the data model can be updated when
    // the user performs actions in the editor widget.
    void setup_connections();

    void setup_menus();

private:
    // the interactive spline editor widget
    SplineEditorWidget*     m_editor_widget;

    // the spline data model.
    SplineDataModel::ptr    m_spline_model;
};
