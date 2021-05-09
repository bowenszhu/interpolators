//*************************************************************************
//    Copyright (c) 2021
//            Bowen Zhu         B.Zhu-3@student.tudelft.nl
//            Mark Khait         M.Khait@tudelft.nl
//            Denis Voskov    D.V.Voskov@tudelft.nl
//    Delft University of Technology, the Netherlands
//
//    This file is part of the Delft Advanced Research Terra Simulator (DARTS)
//
//    DARTS is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as
//    published by the Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.
//
//    DARTS is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with DARTS. If not, see <http://www.gnu.org/licenses/>.
// *************************************************************************

#ifndef BF53EBFF_8376_4642_9B52_6A604A3467C3
#define BF53EBFF_8376_4642_9B52_6A604A3467C3

#include <functional>
#include <vector>
#include "linear_cpu_interpolator_base.h"
#include "static_cpu_interpolator_base.h"

/**
 * Static piecewise linear interpolator
 *
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template <typename index_t, int N_DIMS, int N_OPS>
class LinearStaticCPUInterpolator :
        public LinearCPUInterpolatorBase<index_t, N_DIMS, N_OPS>,
        public StaticCPUInterpolatorBase<index_t, N_DIMS, N_OPS>
{
public:
    /**
     * @brief Construct the interpolator with specified parametrization space
     * 
     * @param[in] supporting_point_evaluator    Object used to compute operators values at supporting points
     * @param[in] axes_points               Number of supporting points (minimum 2) along axes
     * @param[in] axes_min                  Minimum value for each axis
     * @param[in] axes_max                  Maximum for each axis
     */
    LinearStaticCPUInterpolator(operator_set_evaluator_iface *supporting_point_evaluator,
                                const std::array<int, N_DIMS> &axes_points,
                                const std::array<double, N_DIMS> &axes_min,
                                const std::array<double, N_DIMS> &axes_max);

};

#include "linear_static_cpu_interpolator.tpp"
#endif /* BF53EBFF_8376_4642_9B52_6A604A3467C3 */
