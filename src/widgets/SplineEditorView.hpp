#pragma once
#include <QVector>
#include <QGraphicsView>

// A widget that can be used to move spline nodes and display
// the rendered spline.
class SplineEditorView : public QGraphicsView {
Q_OBJECT
public:
    SplineEditorView(QWidget* parent=0);

    SplineEditorView(QGraphicsScene* scene, QWidget* parent=0);

    // Update the rendered spline shown together with the nodes.
    void updateVisualizedSpline(const QVector<QPointF>& points);

public slots:
    void zoomIn()  { scale(1.2,   1.2);   }
    
    void zoomOut() { scale(1/1.2, 1/1.2); }

signals:

    // User clicked on empty space. TODO: Use QPointF
    void userClickedOnEmptySpace(QPointF);

    // User clicked on a node.
    void userClickedOnNode(int id);


protected:
    virtual void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

protected:
    // Collection of lines for the rendered spline curve.
    QVector<QGraphicsItem*>     m_spline_lines;

};
