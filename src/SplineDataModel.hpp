#pragma once
#include <memory>
#include <vector>
#include <QPoint>
#include <QVector>
#include <QString>
#include "KnotVectorType.hpp"

// Forward declaration.
class SplineDataModel;

namespace SplineModelUtils {
    // Load a SplineDataModel instance from a file.
    std::shared_ptr<SplineDataModel> Load(const QString& filename);

    // Create a new SplineDataModel instance.
    std::shared_ptr<SplineDataModel> Create(int degree);

    // Save a SplineDataModel instance to a file.
    void Save(const QString& filename, std::shared_ptr<SplineDataModel> spline_model);
}

class SplineDataModel {
public:
    typedef std::shared_ptr<SplineDataModel>       ptr;
    typedef std::shared_ptr<const SplineDataModel> const_ptr;

    SplineDataModel();

    // Add a new node at the end of the model.
    void add_node(QPointF pos);

    // Update position of an existing node.
    void move_node(int node_index, QPointF new_pos);

    // Remove the node with index *node_index*. Throws if illegal index.
    void delete_node(int node_index);

    // Set the number of points to use when rendering the curve.
    // Throws if negative value.
    void set_render_resolution(int new_res);

    // Render the curve.
    QVector<QPointF> render() const;

    // Set the curve degree
    void set_degree(int degree);

    // Returns all control points.
    QVector<QPointF> get_control_points() const;

    // Set the control points.
    void set_control_points(const QVector<QPointF>& new_points);

    // Set the knot vector type
    void set_knot_vector_type(KnotVectorType type);

    // Returns the knot vector
    QVector<qreal> get_knot_vector() const;

    // Define the parameter range used for spline evaluation.
    void set_eval_limits(qreal t_min, qreal t_max);

    // Define the parameter range used when auto-generating knot vectors.
    void set_autogen_knot_vector_limits(qreal t_min, qreal t_max);

protected:
    // Update the knot vector according to type so that it
    // matches the degree and number of control points.
    void update_knots();

    // Throw exception if node index is illegal.
    void check_node_index(int node_index);

    // Cache the evaluated basis functions
    void precompute_basis_functions();

private:
    QVector<QPointF>            m_control_points;
    QVector<qreal>              m_knots;
    int                         m_degree;
    // The range of parameter values used by render()
    qreal                       m_visualization_start;
    qreal                       m_visualization_stop;
    // The range of parameter values used when auto-generating knot vectors.
    qreal                       m_autogen_knot_start;
    qreal                       m_autogen_knot_stop;
    int                         m_visualization_res;
    QVector<QVector<qreal> >    m_eval_basis; // [time_no][func_no]
    KnotVectorType              m_knot_vector_type;

public:
    friend SplineDataModel::ptr SplineModelUtils::Load(const QString& filename);
    friend SplineDataModel::ptr SplineModelUtils::Create(int degree);
    friend void                 SplineModelUtils::Save(const QString& filename, SplineDataModel::ptr spline_model);
};
