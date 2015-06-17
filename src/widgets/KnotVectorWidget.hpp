#pragma once
#include <QWidget>
#include "KnotVectorType.hpp"

class QTableWidget;

class KnotVectorWidget : public QWidget {
Q_OBJECT
public:
    KnotVectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

signals:
    void knot_vector_type_changed(KnotVectorType type);

private:
    void autogenerate_headers();

private:
    QTableWidget*       m_table_widget;
};