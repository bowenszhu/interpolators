#include "delaunay_adaptive_cpu_interpolator.h"


template<typename index_t, int N_DIMS, int N_OPS>
DelaunayAdaptiveCPUInterpolator<index_t, N_DIMS, N_OPS>::DelaunayAdaptiveCPUInterpolator(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::array<int, N_DIMS> &axes_points,
        const std::array<double, N_DIMS> &axes_min, const std::array<double, N_DIMS> &axes_max)
        : DelaunayCPUInterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator,
                                                              axes_points, axes_min, axes_max),
          AdaptiveCPUInterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator,
                                                              axes_points, axes_min, axes_max),
          InterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min, axes_max) {}
