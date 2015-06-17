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

    // Sets index to "no node selected" and removes values from all
    // input fields.
    void clear();

    void set_index(int node_index);

    void set_position(QPointF pos);

    // Currently unused.
    void set_weight(qreal w);

signals:
    void node_manually_edited(int node_index, QPointF new_pos);

protected slots:
    void something_edited();

private:
    // the index of the node currently being edited.
    int             m_node_index;
    QFormLayout*    m_form_layout;
    QLabel*         m_header_label;
    QLineEdit*      m_x_editor;
    QLineEdit*      m_y_editor;
    QLineEdit*      m_w_editor;
};