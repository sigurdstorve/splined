#pragma once
#include <QWidget>
#include "KnotVectorType.hpp"

class QTableWidget;
class QLineEdit;

class KnotVectorWidget : public QWidget {
Q_OBJECT
public:
    KnotVectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

signals:
    void knot_vector_type_changed(KnotVectorType type);

    // TODO: Consider controlling from another widget
    void eval_interval_changed(qreal t0, qreal t1);

    void auto_knot_limits_changed(qreal t0, qreal t1);

public:
    void update_knots(const QVector<qreal>& new_knots);

private:
    void autogenerate_headers();

    // for convenience.
    void emit_limits();
    void emit_auto_knot_limits();

private:
    QTableWidget*       m_table_widget;
    // To edit which parameter values used when rendering.
    QLineEdit*          m_t0_editor;
    QLineEdit*          m_t1_editor;
    // To edit which min/max parameter value used when auto-generating knot vectors.
    QLineEdit*          m_auto_knot_min_editor;
    QLineEdit*          m_auto_knot_max_editor;
};