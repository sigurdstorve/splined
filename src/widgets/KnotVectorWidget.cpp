#include <QVBoxLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QRadioButton>
#include "KnotVectorWidget.hpp"

KnotVectorWidget::KnotVectorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    auto v_layout = new QVBoxLayout;

    m_table_widget = new QTableWidget(1, 10);
    v_layout->addWidget(m_table_widget);

    auto rb_box = new QVBoxLayout;
    auto group_box = new QGroupBox("Mutually exclusive choices");
    auto rb_clamped = new QRadioButton("Clamped");
    rb_clamped->setChecked(true);
    auto rb_open    = new QRadioButton("Open");
    rb_open->setChecked(false);
    auto rb_closed  = new QRadioButton("Closed");
    rb_closed->setChecked(false);
    auto rb_custom  = new QRadioButton("Custom");
    rb_custom->setChecked(false);
    rb_box->addWidget(rb_clamped);
    rb_box->addWidget(rb_open);
    rb_box->addWidget(rb_closed);
    rb_box->addWidget(rb_custom);
    rb_box->addStretch(1);
    group_box->setLayout(rb_box);
    v_layout->addWidget(group_box);

    setLayout(v_layout);
    autogenerate_headers();
}

void KnotVectorWidget::autogenerate_headers() {
    //QStringList he
    /// TODO:
}
