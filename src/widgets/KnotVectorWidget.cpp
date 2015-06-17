#include <QVBoxLayout>
#include <QTableWidget>
#include "KnotVectorWidget.hpp"

KnotVectorWidget::KnotVectorWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    auto v_layout = new QVBoxLayout;

    m_table_widget = new QTableWidget(1, 10);
    v_layout->addWidget(m_table_widget);
    
    autogenerate_headers();
    
    setLayout(v_layout);
}

void KnotVectorWidget::autogenerate_headers() {
    //QStringList he
    /// TODO:
}