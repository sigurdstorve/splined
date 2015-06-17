#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDoubleValidator>
#include "NodeEditorWidget.hpp"

NodeEditorWidget::NodeEditorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    auto v_layout = new QVBoxLayout;
    auto validator = new QDoubleValidator(-1e6, 1e6, 10, this);

    m_header_label = new QLabel("Node:");
    v_layout->addWidget(m_header_label);

    m_form_layout = new QFormLayout;
    m_x_editor = new QLineEdit;
    m_x_editor->setValidator(validator);
    connect(m_x_editor, SIGNAL(returnPressed()), this, SLOT(something_edited()));
    m_form_layout->addRow("X-coordinate:", m_x_editor);
    m_y_editor = new QLineEdit;
    m_y_editor->setValidator(validator);
    connect(m_y_editor, SIGNAL(returnPressed()), this, SLOT(something_edited()));
    m_form_layout->addRow("Y-coordinate:", m_y_editor);
    m_w_editor = new QLineEdit;
    m_w_editor->setValidator(validator);
    connect(m_w_editor, SIGNAL(returnPressed()), this, SLOT(something_edited()));
    m_form_layout->addRow("Weight:",       m_w_editor);

    v_layout->addLayout(m_form_layout);
    setLayout(v_layout);
    
    clear();
}

void NodeEditorWidget::set_index(int node_index) {
    m_header_label->setText("Node index: " + QString::number(node_index));
    m_node_index = node_index;
}

void NodeEditorWidget::set_position(QPointF pos) {
    m_x_editor->setText(QString::number(pos.x()));
    m_y_editor->setText(QString::number(pos.y()));
}

void NodeEditorWidget::set_weight(qreal w) {
    m_w_editor->setText(QString::number(w));
}

void NodeEditorWidget::clear() {
    m_header_label->setText("No node selected");
    m_x_editor->setText("");
    m_y_editor->setText("");
    m_w_editor->setText("NOT IMPLEMENTED");
    m_node_index = -1;
}

void NodeEditorWidget::something_edited() {
    QPointF new_pos(m_x_editor->text().toDouble(), m_y_editor->text().toDouble());
    qDebug() << "User edited something for node " << m_node_index;
    emit node_manually_edited(m_node_index, new_pos);
}