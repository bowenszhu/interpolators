#ifndef DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H
#define DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H

#include "delaunay_cpu_interpolator_base.hpp"
#include "adaptive_cpu_interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class DelaunayAdaptiveCPUInterpolator :
        public DelaunayCPUInterpolatorBase<index_t, N_DIMS, N_OPS>,
        public AdaptiveCPUInterpolatorBase<index_t, N_DIMS, N_OPS>
{
public:
    DelaunayAdaptiveCPUInterpolator(operator_set_evaluator_iface *supporting_point_evaluator,
                                    const std::array<int, N_DIMS> &axes_points,
                                    const std::array<double, N_DIMS> &axes_min,
                                    const std::array<double, N_DIMS> &axes_max);
};

#include "delaunay_adaptive_cpu_interpolator.tpp"
#endif //DELAUNAY_ADAPTIVE_CPU_INTERPOLATOR_H
