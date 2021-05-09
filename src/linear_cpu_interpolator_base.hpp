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

#ifndef E0E335E5_CB89_46FA_B098_6A9185E06492
#define E0E335E5_CB89_46FA_B098_6A9185E06492

#include <array>

#include "interpolator_base.hpp"

/**
 * @brief  Interpolator base for static/adaptive piecewise linear interpolator.
 * 
 * @tparam index_t index type used for supporting point indexing
 * @tparam N_DIMS The number of dimensions in parameter space
 * @tparam N_OPS The number of operators to be interpolated
 */
template <typename index_t, int N_DIMS, int N_OPS>
class linear_cpu_interpolator_base : virtual public InterpolatorBase<index_t, N_DIMS, N_OPS>
{
public:
    /**
     * @brief Compute interpolation for all operators at the given point
     *
     * @param[in]   point   Coordinates in parametrization space
     * @param[out]  values  Interpolated values
     * @return 0 if interpolation is successful
     */
    int interpolate(const std::vector<double> &point, std::vector<double> &values) override;

    /**
     * @brief Compute interpolation and its gradient for all operators at every specified point
     *
     * @param[in]   points        Array of coordinates in parametrization space
     * @param[in]   points_idxs   Indexes of points in the points array which are marked for interpolation
     * @param[out]  values        Interpolated values
     * @param[out]  derivatives   Interpolation gradients
     * @return 0 if interpolation is successful
     */
    int interpolate_with_derivatives(const std::vector<double> &points, const std::vector<int> &points_idxs,
                                     std::vector<double> &interp_values, std::vector<double> &derivatives) override;

protected:
    /**
     * @brief Construct an interpolator with specified parametrization space
     * 
     * @param[in] supporting_point_evaluator    Object used to compute operators values at supporting points
     * @param[in] axes_points               Number of supporting points (minimum 2) along axes
     * @param[in] axes_min                  Minimum value for each axis
     * @param[in] axes_max                  Maximum for each axis
     */
    linear_cpu_interpolator_base(operator_set_evaluator_iface *supporting_point_evaluator,
                                 const std::array<int, N_DIMS> &axes_points,
                                 const std::array<double, N_DIMS> &axes_min,
                                 const std::array<double, N_DIMS> &axes_max);


    std::array<std::array<int, N_DIMS>, N_DIMS + 1> standard_simplex; ///< a standard simplex

    /**
     * @brief Compute which simplex the given point is located in using standard triangulation
     *
     * @param[in] hypercube The lower-left vertex of the hypercube
     * @param[in] scaled_point The scaled coordinate of the given point inside the hypercube
     * @param[out] tri_order The order of the scaled coordinate which is used for
     *      1. finding simplex for standard triangulation
     *      2. computing weights of the barycentric interpolation
     * @param[out] simplex An array of vertices which forms simplex in N_DIMS-dimensional space
     */
    void find_simplex(const std::array<int, N_DIMS> &hypercube, const std::array<double, N_DIMS> &scaled_point,
                      std::array<int, N_DIMS> &tri_order, std::array<std::array<int, N_DIMS>, N_DIMS + 1> &simplex);
};

#include "linear_cpu_interpolator_base.tpp"
#endif /* E0E335E5_CB89_46FA_B098_6A9185E06492 */
