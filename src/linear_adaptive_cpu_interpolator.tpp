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

#include "linear_adaptive_cpu_interpolator.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
LinearAdaptiveCPUInterpolator<index_t, N_DIMS, N_OPS>::LinearAdaptiveCPUInterpolator(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::array<int, N_DIMS> &axes_points,
        const std::array<double, N_DIMS> &axes_min, const std::array<double, N_DIMS> &axes_max)
        : linear_cpu_interpolator_base<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min,
                                                               axes_max),
          AdaptiveCPUInterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min,
                                                              axes_max),
          InterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min, axes_max) {}
