#include <QDebug>
#include "SplineEditorScene.hpp"

SplineEditorScene::SplineEditorScene(QObject* parent)
    : QGraphicsScene(parent) 
{ }

SplineEditorScene::SplineEditorScene(const QRectF& sceneRect, QObject* parent)
    : QGraphicsScene(sceneRect, parent)
{ }

SplineEditorScene::SplineEditorScene(qreal x, qreal y, qreal width, qreal height, QObject* parent)
    : QGraphicsScene(x, y, width, height, parent)
{ }
