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
class delaunay_cpu_interpolator_base : public interpolator_base
{
public:
    /**
     * @brief Construct an interpolator with specified parametrization space
     *
     * @param[in] supporting_point_evaluator    Object used to compute operators values at supporting points
     * @param[in] axes_points                   Number of supporting points (minimum 2) along axes
     * @param[in] axes_min                      Minimum value for each axis
     * @param[in] axes_max                      Maximum for each axis
     */
    delaunay_cpu_interpolator_base(operator_set_evaluator_iface *supporting_point_evaluator,
                                   const std::vector<int> &axes_points,
                                   const std::vector<double> &axes_min,
                                   const std::vector<double> &axes_max);

    int interpolate(const std::vector<double> &point, std::vector<double> &values) override;

    int interpolate_with_derivatives(const std::vector<double> &points, const std::vector<int> &points_idxs,
                                     std::vector<double> &values, std::vector<double> &derivatives) override;

    int get_n_dims() override;

    int get_n_ops() override;

protected:
    CGAL::Delaunay_triangulation<CGAL::Epick_d<CGAL::Dimension_tag<N_DIMS>>> delaunay_triangulation{N_DIMS};
};

#include "delaunay_cpu_interpolator_base.tpp"
#endif //DELAUNAY_CPU_INTERPOLATOR_BASE_HPP
