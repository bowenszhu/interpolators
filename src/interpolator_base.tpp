#include <numeric>
#include <algorithm>
#include <assert.h>
#include "interpolator_base.h"

template<typename index_t, int N_DIMS, int N_OPS>
InterpolatorBase<index_t, N_DIMS, N_OPS>::InterpolatorBase(operator_set_evaluator_iface *supporting_point_evaluator,
                                                           const std::array<int, N_DIMS> &axes_points,
                                                           const std::array<double, N_DIMS> &axes_min,
                                                           const std::array<double, N_DIMS> &axes_max)
                                                             : supporting_point_evaluator_(supporting_point_evaluator),
                                                               axes_points_(axes_points), axes_min_(axes_min),
                                                               axes_max_(axes_max)
{
    axes_mult_[N_DIMS - 1] = 1;
    for (int dim{N_DIMS - 2}; dim >= 0; dim--)
        axes_mult_[dim] = axes_mult_[dim + 1] * axes_points[dim + 1];

    for (int dim = 0; dim < N_DIMS; dim++)
    {
        axes_step_[dim] = (axes_max[dim] - axes_min[dim]) / (axes_points[dim] - 1);
        axes_step_inv_[dim] = 1.0 / axes_step_[dim];
    }

    /// use double to avoid overflow
    n_points_total_fp_ = std::accumulate(axes_points.begin(), axes_points.end(), 1.0, std::multiplies<double>());

    if (n_points_total_fp_ > std::numeric_limits<index_t>::max())
    {
        std::string error = "Error: The total requested amount of points (" + std::to_string(this->n_points_total_fp_) +
                            ") exceeds the limit in index type (" + std::to_string(std::numeric_limits<index_t>::max())
                            + ")\n";
        throw std::range_error(error);
    }

    n_points_total_ = n_points_total_fp_;
    n_points_used_ = 0;
    n_interpolations_ = 0;
}

template <typename index_t, int N_DIMS, int N_OPS>
void InterpolatorBase<index_t, N_DIMS, N_OPS>::FindHypercube(const std::vector<double> &points,
                                                             std::array<int, N_DIMS> &hypercube,
                                                             std::array<double, N_DIMS> &scaled_point,
                                                             const int point_index)
{
    for (int i = 0; i < N_DIMS; i++)
    {
        scaled_point[i] = (points[point_index + i] - axes_min_[i]) * axes_step_inv_[i];
        hypercube[i] = (int)scaled_point[i];
        scaled_point[i] -= hypercube[i];
    }
}

template<typename index_t, int N_DIMS, int N_OPS>
int InterpolatorBase<index_t, N_DIMS, N_OPS>::evaluate(const std::vector<double> &state, std::vector<double> &values)
{
    timer->start();
    /// call implementation of a derived class
    this->Interpolate(state, values);
    timer->stop();
    n_interpolations_ += N_OPS;
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int InterpolatorBase<index_t, N_DIMS, N_OPS>::evaluate_with_derivatives(const std::vector<double> &states,
                                                                        const std::vector<int> &states_idxs,
                                                                        std::vector<double> &values,
                                                                        std::vector<double> &derivatives)
{
    timer->start();
    /// call implementation of a derived class
    this->InterpolateWithDerivatives(states, states_idxs, values, derivatives);
    timer->stop();
    n_interpolations_ += states_idxs.size() * N_OPS;
    return 0;
}

template<typename index_t, int N_DIMS, int N_OPS>
int InterpolatorBase<index_t, N_DIMS, N_OPS>::get_axis_n_points(int axis)
{
    return axes_points_[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
double InterpolatorBase<index_t, N_DIMS, N_OPS>::get_axis_max(int axis)
{
    return axes_max_[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
double InterpolatorBase<index_t, N_DIMS, N_OPS>::get_axis_min(int axis)
{
    return axes_min_[axis];
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t InterpolatorBase<index_t, N_DIMS, N_OPS>::get_n_interpolations()
{
    return n_interpolations_;
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t InterpolatorBase<index_t, N_DIMS, N_OPS>::get_n_points_total()
{
    return n_points_total_;
}

template<typename index_t, int N_DIMS, int N_OPS>
uint64_t InterpolatorBase<index_t, N_DIMS, N_OPS>::get_n_points_used()
{
    return n_points_used_;
}

template <typename index_t, int N_DIMS, int N_OPS>
void InterpolatorBase<index_t, N_DIMS, N_OPS>::GetVertexFromIndex(index_t index, std::array<int, N_DIMS> &vertex)
{
    for (int dim = 0; dim < N_DIMS; dim++)
    {
        vertex[dim] = index / axes_mult_[dim];
        index %= axes_mult_[dim];
    }
}

template <typename index_t, int N_DIMS, int N_OPS>
void InterpolatorBase<index_t, N_DIMS, N_OPS>::GetPointFromVertex(const std::array<int, N_DIMS> &vertex,
                                                                  std::vector<double> &point)
{
    for (int i = 0; i < N_DIMS; i++)
        point[i] = vertex[i] * axes_step_[i] + axes_min_[i];
}

template <typename index_t, int N_DIMS, int N_OPS>
index_t InterpolatorBase<index_t, N_DIMS, N_OPS>::GetIndexFromVertex(const std::array<int, N_DIMS> &vertex)
{
    return std::inner_product(vertex.begin(), vertex.end(), axes_mult_.begin(), 0);
}
