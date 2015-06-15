#pragma once
#include <QRect>
#include <QObject>
#include <QGraphicsEllipseItem>

class SplineEditorNode : public QObject,
                         public QGraphicsEllipseItem {
Q_OBJECT
public:
    SplineEditorNode(qreal x, qreal y, qreal radius, int node_id, QGraphicsItem* parent=0);

    int get_id() const { return m_node_id; }

signals:

    // The node has changes it's position.
    void itemMoved(int node_id, QPointF pos);

protected:

    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

    // Hijack in order to send out a signal to refresh the spline rendering.
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value) Q_DECL_OVERRIDE;

protected:
    int     m_node_id;
};
