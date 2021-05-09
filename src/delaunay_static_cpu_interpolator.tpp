#include "delaunay_static_cpu_interpolator.hpp"


template<typename index_t, int N_DIMS, int N_OPS>
DelaunayStaticCPUInterpolator<index_t, N_DIMS, N_OPS>::DelaunayStaticCPUInterpolator(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::array<int, N_DIMS> &axes_points,
        const std::array<double, N_DIMS> &axes_min, const std::array<double, N_DIMS> &axes_max)
        : DelaunayCPUInterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator,
                                                              axes_points, axes_min, axes_max),
          static_cpu_interpolator_base<index_t, N_DIMS, N_OPS>(supporting_point_evaluator,
                                                               axes_points, axes_min, axes_max),
          interpolator_base<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min, axes_max) {}