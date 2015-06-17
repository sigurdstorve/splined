#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include "NodeEditorWidget.hpp"

// TODO: Use QValidator

NodeEditorWidget::NodeEditorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f) 
{
    auto v_layout = new QVBoxLayout;

    m_header_label = new QLabel("Node:");
    v_layout->addWidget(m_header_label);

    m_form_layout = new QFormLayout;
    m_x_editor = new QLineEdit;
    m_form_layout->addRow("X-coordinate:", m_x_editor);
    m_y_editor = new QLineEdit;
    m_form_layout->addRow("Y-coordinate:", m_y_editor);
    m_w_editor = new QLineEdit;
    m_form_layout->addRow("Weight:",       m_w_editor);

    v_layout->addLayout(m_form_layout);
    setLayout(v_layout);
}

void NodeEditorWidget::set_index(int node_index) {
    m_header_label->setText("Node index: " + QString::number(node_index));
}

void NodeEditorWidget::set_position(QPointF pos) {
    m_x_editor->setText(QString::number(pos.x()));
    m_y_editor->setText(QString::number(pos.y()));
}

void NodeEditorWidget::set_weight(qreal w) {
    m_w_editor->setText(QString::number(w));
}