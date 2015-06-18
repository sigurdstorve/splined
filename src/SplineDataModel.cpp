#include <cmath>
#include <QDebug>
#include <stdexcept>
#include <QFile>
#include <QStringList>
#include "bspline.hpp"
#include "SplineDataModel.hpp"

namespace detail {

// Convenience function to write a vector of real values to a stream.
void write(QTextStream& stream, const QVector<qreal>& values) {
    const auto num_values = values.size();
    for (int i = 0; i < num_values; i++) {
        stream << values[i] << " ";
    }
}
    
// Convenience function to split a vector of 2D points into separate
// vectors containing x- and y-components.
void split_xs_ys(const QVector<QPointF>& points, QVector<qreal>& /*out*/ xs, QVector<qreal>& /*out*/ ys) {
    const auto num_points = points.size();
    xs.resize(num_points);
    ys.resize(num_points);
    for (int i = 0; i < num_points; i++) {
        xs[i] = points[i].x();
        ys[i] = points[i].y();
    }
}

// The inverse of split_xs_ys().
QVector<QPointF> combine_xs_ys(const QVector<qreal>& xs, const QVector<qreal>& ys) {
    const auto num_points = xs.size();
    if (ys.size() != num_points) {
        throw std::runtime_error("Size mistmatch");
    }
    QVector<QPointF> res(num_points);
    for (int i = 0; i < num_points; i++) {
        res[i] = QPointF(xs[i], ys[i]);
    }
    return res;
}

// Convenience function to extract a vector of qreal from a string.
QVector<qreal> tokenize(const QString& tokens) {
    auto temp = tokens.split(" ");
    
    const auto num_tokens = temp.size();
    QVector<qreal> res;
    for (int i = 0; i < num_tokens; i++) {
        if (temp[i] != "") {
            res.push_back( static_cast<qreal>(temp[i].toDouble()) );
        }
    }
    return res;
}

}   // end namespace detail


SplineDataModel::SplineDataModel() 
    : m_visualization_res(1000),
        m_visualization_start(0.0),
        m_visualization_stop(1.0),
        m_autogen_knot_start(0.0),
        m_autogen_knot_stop(1.0),
        m_degree(0),
        m_knot_vector_type(KnotVectorType::CLAMPED)
{
}

void SplineDataModel::add_node(QPointF pos) {
    m_control_points.append(pos);
    update_knots();
    precompute_basis_functions();
}

void SplineDataModel::move_node(int node_index, QPointF new_pos) {
    check_node_index(node_index);
    m_control_points[node_index] = new_pos;
}

void SplineDataModel::delete_node(int node_index) {
    check_node_index(node_index);
    m_control_points.erase(m_control_points.begin() + node_index);
    update_knots();
    precompute_basis_functions();
}

void SplineDataModel::set_render_resolution(int new_res) {
    if (new_res < 0) {
        throw std::runtime_error("Render resolution cannot be negative");
    }
    m_visualization_res = new_res;
    precompute_basis_functions();
}

QVector<QPointF> SplineDataModel::render() const {
    
    QVector<QPointF> res(m_visualization_res);
    const auto num_control_points = m_control_points.size();

    for (int point_idx = 0; point_idx < m_visualization_res; point_idx++) {
        QPointF p(0.0, 0.0);
        for (int i = 0; i < num_control_points; i++) {
            p += m_eval_basis[point_idx][i]*m_control_points[i];
        }
        res[point_idx] = p;
    }
    return res;
}

void SplineDataModel::set_degree(int degree) {
    if (degree < 0) {
        throw std::runtime_error("Illegal spline degree");
    }
    m_degree = degree;
}

QVector<QPointF> SplineDataModel::get_control_points() const {
    return m_control_points;
}

void SplineDataModel::set_control_points(const QVector<QPointF>& new_points) {
    m_control_points = new_points;
    update_knots();
    precompute_basis_functions();
}

void SplineDataModel::update_knots() {
    using namespace bspline_storve;

    const auto t0 = m_autogen_knot_start;
    const auto t1 = m_autogen_knot_stop;
    const auto num_points = m_control_points.size();
    
    std::vector<qreal> temp_knots;
    switch(m_knot_vector_type) {
    case KnotVectorType::CLAMPED:
        temp_knots = uniformRegularKnotVector(num_points, m_degree, t0, t1, true);
        break;
    case KnotVectorType::CLOSED:
        qDebug() << "WARNING: t0 and t1 are ignore. Currently assumed to be [0, 1]";
        temp_knots = closedKnotVector<qreal>(num_points, m_degree);
        break;
    case KnotVectorType::OPEN:
        temp_knots = uniformOpenKnotVector(num_points, m_degree, t0, t1, true);
        break;
    case KnotVectorType::CUSTOM:
        throw std::runtime_error("Custom knot vector support not yet implemented");
        break;
    default:
        throw std::runtime_error("Unknown knot vector type.");
    }

    m_knots = QVector<qreal>::fromStdVector(temp_knots);
}

void SplineDataModel::check_node_index(int node_index) {
    if (node_index < 0 || node_index >= m_control_points.size()) {
        throw std::runtime_error("Illegal node index");
    }
}

void SplineDataModel::precompute_basis_functions() {
    const auto temp_knots = m_knots.toStdVector();

    m_eval_basis.resize(m_visualization_res);
    const auto num_functions = m_control_points.size();
    for (int time_no = 0; time_no < m_visualization_res; time_no++) {
        m_eval_basis[time_no].resize(num_functions);
        const double cur_time = m_visualization_start + time_no*(m_visualization_stop-m_visualization_start)/(m_visualization_res-1.0);
        for (int func_no = 0; func_no < num_functions; func_no++) {
            m_eval_basis[time_no][func_no] = bspline_storve::bsplineBasis(func_no, m_degree, cur_time, temp_knots);
        }
    }
}

void SplineDataModel::set_knot_vector_type(KnotVectorType type) {
    m_knot_vector_type = type;
    update_knots();
    precompute_basis_functions();
}

void SplineDataModel::set_eval_limits(qreal t_min, qreal t_max) {
    m_visualization_start = t_min;
    m_visualization_stop  = t_max;
    precompute_basis_functions();
}

void SplineDataModel::set_autogen_knot_vector_limits(qreal t_min, qreal t_max) {
    m_autogen_knot_start = t_min;
    m_autogen_knot_stop  = t_max;
    update_knots();
    precompute_basis_functions();
}

QVector<qreal> SplineDataModel::get_knot_vector() const {
    return m_knots;
}

SplineDataModel::ptr SplineModelUtils::Load(const QString& filename) {
    SplineDataModel::ptr loaded_model = SplineDataModel::ptr(new SplineDataModel);
    
    QVector<qreal> temp_xs;
    QVector<qreal> temp_ys;

    QFile in_file(filename);
    if (in_file.open(QIODevice::ReadOnly)) {
        QTextStream in_stream(&in_file);
        while (!in_stream.atEnd()) {
            QString line = in_stream.readLine();

            // extract keyword and rest
            QStringList list = line.split(":");
            if (list.size() != 2) {
                qDebug() << "Ignoring line: " << line;
                continue;
            }
            auto keyword = list[0];
            auto rest    = list[1];
            //qDebug() << "keyword: " << keyword << ", rest: " << rest;

            if (keyword == "version") {
                if (rest.replace(" ", "") != "1.0") {
                    qDebug() << "Unsupported file format version: " << rest;
                    return nullptr;
                }
            } else if (keyword == "degree") {
                loaded_model->set_degree(rest.toInt());
            } else if (keyword == "knots") {
                loaded_model->m_knots = detail::tokenize(rest);
            } else if (keyword == "x") {
                temp_xs = detail::tokenize(rest);
            } else if (keyword == "y") {
                temp_ys = detail::tokenize(rest);
            } else if (keyword == "t0") {
                loaded_model->m_visualization_start = rest.toDouble();
            } else if (keyword == "t1") {
                loaded_model->m_visualization_stop  = rest.toDouble();
            } else {
                qDebug() << "Ignoring unknown keyword: " << keyword;
            }
        }
        in_file.close();
    } else {
        qDebug() << "Unable to open file for reading";
    }

    // Combine into a vector of QPointF
    loaded_model->set_control_points(detail::combine_xs_ys(temp_xs, temp_ys));

    return loaded_model;
}

SplineDataModel::ptr SplineModelUtils::Create(int degree) {
    auto spline_model = SplineDataModel::ptr(new SplineDataModel);

    // Generate the least number of nodes that is required for the given degree
    // for a clamped knot vector:
    // num_knots = n + (p+1) = (p+1) + (p+1) so n = p+1

    QVector<QPointF> cs;
    const auto radius     = 0.5;
    const auto TWO_PI     = 8*std::atan(1);
    const auto num_points = degree+1;
    for (int point_no = 0; point_no < num_points; point_no++) {
        const auto angle = point_no*TWO_PI/num_points;
        cs.append(QPointF(radius*std::cos(angle), radius*std::sin(angle)));
    }

    spline_model->set_degree(degree);
    spline_model->set_control_points(cs);

    return spline_model;
}

void SplineModelUtils::Save(const QString& filename, SplineDataModel::ptr spline_model) {
    QFile out_file(filename);
    if (out_file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&out_file);

        stream << "version: 1.0\n";
        stream << "degree: " << spline_model->m_degree << "\n";
        stream << "knots: ";
        detail::write(stream, spline_model->m_knots);
        stream << "\n";
        QVector<qreal> xs, ys;
        detail::split_xs_ys(spline_model->m_control_points, xs, ys);
        stream << "x: ";
        detail::write(stream, xs);
        stream << "\n";
        stream << "y: ";
        detail::write(stream, ys);
        stream << "\n";
        stream << "t0: " << spline_model->m_visualization_start << "\n";
        stream << "t1: " << spline_model->m_visualization_stop  << "\n";

        out_file.close();
    } else {
        qDebug() << "Unable to open file for writing.";
    }
}

int SplineDataModel::get_degree() const {
    return m_degree;
}
