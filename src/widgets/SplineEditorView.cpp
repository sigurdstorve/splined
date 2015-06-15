#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QGraphicsLineItem>
#include "SplineEditorView.hpp"

SplineEditorView::SplineEditorView(QWidget* parent)
    : QGraphicsView(parent)
{ }

SplineEditorView::SplineEditorView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{ }

void SplineEditorView::mousePressEvent(QMouseEvent * event) {
    const auto scene_pos = mapToScene(event->pos());
    const auto clicked_item = scene()->itemAt(scene_pos, transform());
    if (clicked_item == 0) {
        emit userClickedOnEmptySpace(scene_pos);
    } else {
        emit userClickedOnNode(1);
        QGraphicsView::mousePressEvent(event);
    }
}

void SplineEditorView::wheelEvent(QWheelEvent* event) {
    if (event->delta() > 0) {
        zoomIn();        
    } else {
        zoomOut();
    }
}

void SplineEditorView::updateVisualizedSpline(const QVector<QPointF>& points) {
    qDebug() << "deleting";
    for (QGraphicsItem* item : m_spline_lines) {
        scene()->removeItem(item);
    }
    m_spline_lines.clear();

    auto num_lines = points.size();
    if (num_lines <= 1) {
        qDebug() << "Too few lines";
        return;
    }

    QPen line_pen;
    line_pen.setWidthF(0);

    qDebug() << "recreating";
    for (int i = 0; i < num_lines-1; i++) {
        const auto& p1 = points[i];
        const auto& p2 = points[i+1];
        auto line_item = new QGraphicsLineItem(p1.x(), p1.y(), p2.x(), p2.y());
        scene()->addItem(line_item);
        line_item->setPen(line_pen);
        m_spline_lines.append(line_item);
    }

}
