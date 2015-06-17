#pragma once
#include <QWidget>
#include "SplineDataModel.hpp"

class SplineEditorView;
class SplineEditorScene;
class QGraphicsItem;
class NodeEditorWidget;
class KnotVectorWidget;

class SplineEditorWidget : public QWidget {
Q_OBJECT
public:
    SplineEditorWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

signals:
    // user has requested to add a new node
    void node_added(QPointF pos);

    // user has requested to move a node
    void node_moved(int node_index, QPointF new_pos);

    // user has requested to delete a node
    void node_deleted(int node_index);

public slots:
    // user has marked a node by clicking on it.
    void on_node_selected(int node_index);

public:
    
    // update the nodes according to the data model, e.g. to relect
    // the addition or deletion of a node (but not movement)
    void update_from_model(SplineDataModel::ptr model);

    // update the rendered spline curve.
    void update_rendered_spline(const QVector<QPointF>& points);

    // returns the index of the node that is currently selected,
    // or -1 if no node is selected.
    int get_selected_node() const;

protected slots:
    // The user has edited a node from the node editor widget.
    void on_node_edited();

protected:
    void draw_coordinate_axes();

    void setup_connections();

    void remove_old_items(QVector<QGraphicsItem*>& items);

private:
    SplineEditorView*       m_view;
    SplineEditorScene*      m_scene;
    QVector<QGraphicsItem*> m_node_items;
    QVector<QGraphicsItem*> m_line_items;
    NodeEditorWidget*       m_node_editor;
    KnotVectorWidget*       m_knot_vector_editor;
};