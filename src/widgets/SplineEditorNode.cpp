#include <cmath>
#include <QDebug>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include "SplineEditorNode.hpp"

SplineEditorNode::SplineEditorNode(qreal x, qreal y, qreal radius, int node_id, QGraphicsItem* parent)
    : QObject(),
      QGraphicsEllipseItem(-radius, -radius, 2.0*radius, 2.0*radius, parent),
      m_node_id(node_id)
{
    auto number = new QGraphicsSimpleTextItem(QString::number(node_id), this);
    auto bounding_rect = number->boundingRect();
    auto scale_factor = std::max(bounding_rect.width(), bounding_rect.height());
    number->setScale(2.0*radius/scale_factor);
    number->setPos(radius, radius);

    // Center it in its local coordinate system so that its position (in parent coordinate
    // system) is equal to the center of circle.
    setPos(x, y);
}

void SplineEditorNode::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete) {
        emit itemDeleteRequest(m_node_id);
        event->accept();
    }
}

void SplineEditorNode::mouseReleaseEvent(QGraphicsSceneMouseEvent * event) {
    emit itemSelected(m_node_id);
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

QVariant SplineEditorNode::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemPositionChange) {
        emit itemMoved(m_node_id, pos());
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void SplineEditorNode::set_position(int /*node_index*/, QPointF new_pos) {
    setPos(new_pos);
}
