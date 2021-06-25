#ifndef STATIC_CPU_INTERPOLATOR_HPP
#define STATIC_CPU_INTERPOLATOR_HPP

#include "interpolator_base.h"

/**
 * @brief  Base class for interpolators with static storage.
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template<typename index_t, int N_DIMS, int N_OPS>
class StaticCPUInterpolatorBase
        : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS> {
protected:
    StaticCPUInterpolatorBase(
            operator_set_evaluator_iface *supportingPointEvaluator,
            const std::array<int, N_DIMS> &axesPoints,
            const std::array<double, N_DIMS> &axesMin,
            const std::array<double, N_DIMS> &axesMax);

    /// static storage: the values of operators at all supporting points
    std::vector<double> point_data_;

    /**
     * @brief Get the operator values at the given supporting point using
     *        static storage
     *
     * @param[in] vertex The indexes of coordinates the given supporting
     *                   point along axes
     * @param[out] values The values of operators at a given point
     */
    void GetSupportingPoint(const std::array<int, N_DIMS> &vertex,
                            std::array<double, N_OPS> &values) override;
};

#include "static_cpu_interpolator_base.tpp"

#endif //STATIC_CPU_INTERPOLATOR_HPP
