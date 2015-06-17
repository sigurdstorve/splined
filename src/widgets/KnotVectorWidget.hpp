#pragma once
#include <QWidget>

class QTableWidget;

class KnotVectorWidget : public QWidget {
Q_OBJECT
public:
    KnotVectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

private:
    void autogenerate_headers();

private:
    QTableWidget*       m_table_widget;
};