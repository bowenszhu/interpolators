#ifndef DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H
#define DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H

#include "delaunay_cpu_interpolator_base.h"
#include "adaptive_cpu_interpolator_base.h"

/**
 * @brief  Barycentric interpolator with Delaunay triangulation and
 *         adaptive storage.
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template<typename index_t, int N_DIMS, int N_OPS>
class DelaunayAdaptiveCPUInterpolator :
        public DelaunayCPUInterpolatorBase<index_t, N_DIMS, N_OPS>,
        public AdaptiveCPUInterpolatorBase<index_t, N_DIMS, N_OPS> {
public:
    DelaunayAdaptiveCPUInterpolator(
            operator_set_evaluator_iface *supporting_point_evaluator,
            const std::array<int, N_DIMS> &axes_points,
            const std::array<double, N_DIMS> &axes_min,
            const std::array<double, N_DIMS> &axes_max);
};

#include "delaunay_adaptive_cpu_interpolator.tpp"

#endif //DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H
