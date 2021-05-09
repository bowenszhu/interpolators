#ifndef DELAUNAY_STATIC_CPU_INTERPOLATOR_H
#define DELAUNAY_STATIC_CPU_INTERPOLATOR_H

#include "delaunay_cpu_interpolator_base.hpp"
#include "static_cpu_interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class DelaunayStaticCPUInterpolator :
        public DelaunayCPUInterpolatorBase<index_t, N_DIMS, N_OPS>,
        public static_cpu_interpolator_base<index_t, N_DIMS, N_OPS>
{
public:
    DelaunayStaticCPUInterpolator(operator_set_evaluator_iface *supporting_point_evaluator,
                                  const std::array<int, N_DIMS> &axes_points,
                                  const std::array<double, N_DIMS> &axes_min,
                                  const std::array<double, N_DIMS> &axes_max);
};

#include "delaunay_static_cpu_interpolator.tpp"
#endif //DELAUNAY_STATIC_CPU_INTERPOLATOR_H
