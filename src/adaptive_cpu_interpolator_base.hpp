#ifndef ADAPTIVE_CPU_INTERPOLATOR_HPP
#define ADAPTIVE_CPU_INTERPOLATOR_HPP

#include "interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class AdaptiveCPUInterpolatorBase : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS>
{
protected:
    AdaptiveCPUInterpolatorBase(operator_set_evaluator_iface *supportingPointEvaluator,
                                const std::array<int, N_DIMS> &axesPoints, const std::array<double, N_DIMS> &axesMin,
                                const std::array<double, N_DIMS> &axesMax);

    std::unordered_map<index_t, std::array<double, N_OPS>> point_data_; ///< adaptive storage: the values of operators at supporting points actually required

    /**
     * @brief Get values of operators at supporting point
     * The function checks whether the values at the given vertex are computed before,
     *      if yes, the values are directly returned;
     *      if not, the function computes the values, stores them and then returns.
     *
     * @param[in] vertex The index of the given supporting point along axes
     * @param[out] values The values of operators at a given point
     */
    void GetSupportingPoint(const std::array<int, N_DIMS> &vertex, std::array<double, N_OPS> &values) override;
};

#include "adaptive_cpu_interpolator_base.tpp"
#endif //ADAPTIVE_CPU_INTERPOLATOR_HPP
