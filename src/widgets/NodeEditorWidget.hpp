#pragma once
#include <QWidget>

class QFormLayout;
class QLineEdit;
class QLabel;
class QMouseEvent;

class NodeEditorWidget : public QWidget {
Q_OBJECT
public:
    NodeEditorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

    void set_node_index(int node_index);

private:
    QFormLayout*    m_form_layout;
    QLabel*         m_header_label;
    QLineEdit*      m_x_editor;
    QLineEdit*      m_y_editor;
    QLineEdit*      m_w_editor;
};