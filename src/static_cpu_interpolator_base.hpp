#ifndef STATIC_CPU_INTERPOLATOR_HPP
#define STATIC_CPU_INTERPOLATOR_HPP

#include "interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
class static_cpu_interpolator_base : virtual public interpolator_base<index_t, N_DIMS, N_OPS>
{

};

#include "static_cpu_interpolator_base.tpp"
#endif //STATIC_CPU_INTERPOLATOR_HPP
