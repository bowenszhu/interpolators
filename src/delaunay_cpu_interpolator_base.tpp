#include "delaunay_cpu_interpolator_base.hpp"
#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>
#include <Eigen/Dense>


template<typename index_t, int N_DIMS, int N_OPS>
delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::delaunay_cpu_interpolator_base(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::array<int, N_DIMS> &axes_points,
        const std::array<double, N_DIMS> &axes_min, const std::array<double, N_DIMS> &axes_max)
        : interpolator_base<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min, axes_max)
{
    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>> T;
    std::array<typename T::Point, 1 << N_DIMS> points; /// a hypercube has 2 ^ dim vertices in total
    for (int j = 0; j < points.size(); ++j)
    {
        std::bitset<N_DIMS> binary (j);
        double point[N_DIMS];
        for (int i = 0; i < N_DIMS; i++)
            point[i] = binary[i];
        typename T::Point p(&point[0], &point[N_DIMS]);
        points[j] = p;
    }
    delaunay_triangulation.insert(points.begin(), points.end());
}

template<typename index_t, int N_DIMS, int N_OPS>
int delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::interpolate(const std::vector<double> &point,
                                                                        std::vector<double> &values)
{
    std::array<int, N_DIMS> hypercube;
    std::array<double, N_DIMS> scaled_point;
    interpolator_base<index_t, N_DIMS, N_OPS>::find_hypercube(point, hypercube, scaled_point);

    Eigen::Matrix<double, N_DIMS + 1, 1> weights;
    std::array<std::array<int, N_DIMS>, N_DIMS + 1> simplex;
    find_simplex(hypercube, scaled_point, weights, simplex);

    values.resize(N_OPS);
    for (int dim_i = 0; dim_i <= N_DIMS; dim_i++)
    {
        std::array<double, N_OPS> supp_values;
        this->get_supporting_point(simplex[dim_i], supp_values);
        for (int op_i = 0; op_i < N_OPS; op_i++)
            values[op_i] += weights[dim_i] * supp_values[op_i];
    }

    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int
delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::interpolate_with_derivatives(const std::vector<double> &points,
                                                                                     const std::vector<int> &points_idxs,
                                                                                     std::vector<double> &values,
                                                                                     std::vector<double> &derivatives) {
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
void delaunay_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::find_simplex(const std::array<int, N_DIMS> &hypercube,
        const std::array<double, N_DIMS> &scaled_point, Eigen::Matrix<double, N_DIMS + 1, 1> &weights,
        std::array<std::array<int, N_DIMS>, N_DIMS + 1> &simplex)
{
    typedef CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>> T;
    typename T::Point t_scaled_point(scaled_point.begin(), scaled_point.end());
    typename T::Full_cell_handle handle = delaunay_triangulation.locate(t_scaled_point);

    Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> vertex_matrix;
    vertex_matrix.row(N_DIMS) = Eigen::Matrix<double, 1, N_DIMS + 1>::Constant(1.0);

    for (int vertex_i = 0; vertex_i <= N_DIMS; vertex_i++)
    {
        typename T::Point vertex = handle->vertex(vertex_i)->point();
        for (int dim_i = 0; dim_i < N_DIMS; dim_i++)
        {
            simplex[vertex_i][dim_i] = hypercube[dim_i] + vertex[dim_i];
            vertex_matrix(dim_i, vertex_i) = vertex[dim_i];
        }
    }

    Eigen::Matrix<double, N_DIMS + 1, 1> scaled_point_vector;
    for (int dim_i = 0; dim_i < N_DIMS; dim_i++)
        scaled_point_vector[dim_i] = scaled_point[dim_i];
    scaled_point_vector[N_DIMS] = 1.0;

    weights = vertex_matrix.colPivHouseholderQr().solve(scaled_point_vector);
}
