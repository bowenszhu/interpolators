#ifndef ADAPTIVE_CPU_INTERPOLATOR_HPP
#define ADAPTIVE_CPU_INTERPOLATOR_HPP

#include "interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class adaptive_cpu_interpolator_base : virtual public interpolator_base<index_t, N_DIMS, N_OPS>
{

};

#include "adaptive_cpu_interpolator_base.tpp"
#endif //ADAPTIVE_CPU_INTERPOLATOR_HPP
