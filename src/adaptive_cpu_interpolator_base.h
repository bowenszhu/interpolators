#ifndef ADAPTIVE_CPU_INTERPOLATOR_H
#define ADAPTIVE_CPU_INTERPOLATOR_H

#include "interpolator_base.h"

/**
 * @brief  Base class for interpolators with adaptive storage.
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template<typename index_t, int N_DIMS, int N_OPS>
class AdaptiveCPUInterpolatorBase
        : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS> {
protected:
    AdaptiveCPUInterpolatorBase(
            operator_set_evaluator_iface *supportingPointEvaluator,
            const std::array<int, N_DIMS> &axesPoints,
            const std::array<double, N_DIMS> &axesMin,
            const std::array<double, N_DIMS> &axesMax);

    /// adaptive storage: the values of operators at supporting points
    /// actually required
    std::unordered_map<index_t, std::array<double, N_OPS>> point_data_;

    /**
     * @brief Get values of operators at supporting point
     * The function checks whether the values at the given vertex are
     * computed before,
     *   if yes, the values are directly returned;
     *   if not, the function computes the values, stores them and then returns.
     *
     * @param[in]  vertex The index of the given supporting point along axes
     * @param[out] values The values of operators at a given point
     */
    void GetSupportingPoint(const std::array<int, N_DIMS> &vertex,
                            std::array<double, N_OPS> &values) override;
};

#include "adaptive_cpu_interpolator_base.tpp"

#endif //ADAPTIVE_CPU_INTERPOLATOR_H
