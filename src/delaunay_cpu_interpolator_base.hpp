#ifndef DELAUNAY_CPU_INTERPOLATOR_BASE_HPP
#define DELAUNAY_CPU_INTERPOLATOR_BASE_HPP


#include <CGAL/Delaunay_triangulation.h>
#include <CGAL/Epick_d.h>
#include "interpolator_base.hpp"

/**
 * @brief  Interpolator base for static/adaptive piecewise linear interpolator.
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template <typename index_t, int N_DIMS, int N_OPS>
class DelaunayCPUInterpolatorBase : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS>
{
public:
    int Interpolate(const std::vector<double> &point, std::vector<double> &values) override;

    int interpolate_with_derivatives(const std::vector<double> &points, const std::vector<int> &points_idxs,
                                     std::vector<double> &values, std::vector<double> &derivatives) override;

protected:
    /**
     * @brief Construct an interpolator with specified parametrization space
     *
     * @param[in] supporting_point_evaluator    Object used to compute operators values at supporting points
     * @param[in] axes_points                   Number of supporting points (minimum 2) along axes
     * @param[in] axes_min                      Minimum value for each axis
     * @param[in] axes_max                      Maximum for each axis
     */
    DelaunayCPUInterpolatorBase(operator_set_evaluator_iface *supporting_point_evaluator,
                                const std::array<int, N_DIMS> &axes_points,
                                const std::array<double, N_DIMS> &axes_min,
                                const std::array<double, N_DIMS> &axes_max);

    CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>> delaunay_triangulation_{N_DIMS};

    void find_simplex(const std::array<int, N_DIMS> &hypercube, const std::array<double, N_DIMS> &scaled_point,
                      Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
                      std::array<std::array<int, N_DIMS>, N_DIMS + 1> &simplex);

    void compute_barycentric_coord(const Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
                                   const std::array<double, N_DIMS> &scaled_point,
                                   Eigen::Matrix<double, N_DIMS + 1, 1> &weights);

    void compute_barycentric_coord_and_vertex_matrix_inv(const Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix,
                                                         const std::array<double, N_DIMS> &scaled_point,
                                                         Eigen::Matrix<double, N_DIMS + 1, 1> &weights,
                                                         Eigen::Matrix<double, N_DIMS + 1, N_DIMS + 1> &vertex_matrix_inv);
};

#include "delaunay_cpu_interpolator_base.tpp"
#endif //DELAUNAY_CPU_INTERPOLATOR_BASE_HPP
