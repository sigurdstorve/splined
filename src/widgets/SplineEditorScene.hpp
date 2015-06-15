#pragma once
#include <QGraphicsScene>

class SplineEditorScene : public QGraphicsScene {
Q_OBJECT
public:
    SplineEditorScene(QObject* parent=0);

    SplineEditorScene(const QRectF& sceneRect, QObject* parent=0);

    SplineEditorScene(qreal x, qreal y, qreal width, qreal height, QObject* parent=0);
};
