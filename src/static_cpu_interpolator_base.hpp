#ifndef STATIC_CPU_INTERPOLATOR_HPP
#define STATIC_CPU_INTERPOLATOR_HPP

#include "interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class static_cpu_interpolator_base : virtual public interpolator_base<index_t, N_DIMS, N_OPS>
{
protected:
public:
    static_cpu_interpolator_base(operator_set_evaluator_iface *supportingPointEvaluator,
                                 const std::array<int, N_DIMS> &axesPoints, const std::array<double, N_DIMS> &axesMin,
                                 const std::array<double, N_DIMS> &axesMax);

protected:
    std::vector<double> point_data; ///< static storage: the values of operators at all supporting points
};

#include "static_cpu_interpolator_base.tpp"
#endif //STATIC_CPU_INTERPOLATOR_HPP
