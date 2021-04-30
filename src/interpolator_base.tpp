#include <numeric>
#include <algorithm>
#include <assert.h>
#include "interpolator_base.hpp"

template<typename index_t, int N_DIMS, int N_OPS>
interpolator_base<index_t, N_DIMS, N_OPS>::interpolator_base(operator_set_evaluator_iface *supporting_point_evaluator,
                                                             const std::array<int, N_DIMS> &axes_points,
                                                             const std::array<double, N_DIMS> &axes_min,
                                                             const std::array<double, N_DIMS> &axes_max)
                                                             : supporting_point_evaluator(supporting_point_evaluator),
                                                             axes_points(axes_points), axes_min(axes_min),
                                                             axes_max(axes_max)
{
    for (int dim = 0; dim < N_DIMS; dim++)
    {
        axes_step[dim] = (axes_max[dim] - axes_min[dim]) / (axes_points[dim] - 1);
        axes_step_inv[dim] = 1.0 / axes_step[dim];
    }

    /// use double to avoid overflow
    n_points_total_fp = std::accumulate(axes_points.begin(), axes_points.end(), 1.0, std::multiplies<double>());

    n_points_total = n_points_total_fp;
    n_points_used = 0;
    n_interpolations = 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int interpolator_base<index_t, N_DIMS, N_OPS>::init()
{
    /// take n_dims from derived interpolator class
    int n_dims_interpolator = this->get_n_dims();
    /// take n_dims from parameter space passed during construction
    int n_dims_parameter_space = N_DIMS;
    /// make sure they match
    assert(n_dims_interpolator == n_dims_parameter_space);
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int interpolator_base<index_t, N_DIMS, N_OPS>::evaluate(const std::vector<double> &state, std::vector<double> &values)
{
    timer->start();
    /// call implementation of a derived class
    this->interpolate(state, values);
    timer->stop();
    n_interpolations += N_OPS;
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int interpolator_base<index_t, N_DIMS, N_OPS>::evaluate_with_derivatives(const std::vector<double> &states,
                                                 const std::vector<int> &states_idxs,
                                                 std::vector<double> &values,
                                                 std::vector<double> &derivatives)
{
    timer->start();
    /// call implementation of a derived class
    this->interpolate_with_derivatives(states, states_idxs, values, derivatives);
    timer->stop();
    n_interpolations += states_idxs.size() * N_OPS;
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int interpolator_base<index_t, N_DIMS, N_OPS>::get_axis_n_points(int axis)
{
    return axes_points[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
double interpolator_base<index_t, N_DIMS, N_OPS>::get_axis_max(int axis)
{
    return axes_max[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
double interpolator_base<index_t, N_DIMS, N_OPS>::get_axis_min(int axis)
{
    return axes_min[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t interpolator_base<index_t, N_DIMS, N_OPS>::get_n_interpolations()
{
    return n_interpolations;
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t interpolator_base<index_t, N_DIMS, N_OPS>::get_n_points_total()
{
    return n_points_total;
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t interpolator_base<index_t, N_DIMS, N_OPS>::get_n_points_used()
{
    return n_points_used;
}
