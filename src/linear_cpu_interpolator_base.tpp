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

#include <numeric>
#include <algorithm>
#include "linear_cpu_interpolator_base.hpp"
#include "static_cpu_interpolator_base.hpp"

template <typename index_t, int N_DIMS, int N_OPS>
LinearCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::LinearCPUInterpolatorBase(
        operator_set_evaluator_iface *supporting_point_evaluator, const std::array<int, N_DIMS> &axes_points,
        const std::array<double, N_DIMS> &axes_min, const std::array<double, N_DIMS> &axes_max)
        : InterpolatorBase<index_t, N_DIMS, N_OPS>(supporting_point_evaluator, axes_points, axes_min, axes_max)
{
    /// initialize the values with 0's
    standard_simplex_ = {};
    /// and then set upper triangular elements to 1's
    for (int vertex_i = 0; vertex_i < N_DIMS; vertex_i++)
        for (int dim_i = vertex_i; dim_i < N_DIMS; dim_i++)
            standard_simplex_[vertex_i][dim_i] = 1;
}

template <typename index_t, int N_DIMS, int N_OPS>
void LinearCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::find_simplex(const std::array<int, N_DIMS> &hypercube,
                                                                     const std::array<double, N_DIMS> &scaled_point,
                                                                     std::array<int, N_DIMS> &tri_order,
                                                                     std::array<std::array<int, N_DIMS>, N_DIMS + 1> &simplex)
{
    std::iota(tri_order.begin(), tri_order.end(), 0);
    std::sort(tri_order.begin(), tri_order.end(),
              [&scaled_point](int i1, int i2) { return scaled_point[i1] < scaled_point[i2]; });

    for (int vertex_i = 0; vertex_i <= N_DIMS; vertex_i++)
    {
        for (int dim_i = 0; dim_i < N_DIMS; dim_i++)
            simplex[vertex_i][dim_i] = hypercube[dim_i];
        for (int dim_i = 0; dim_i < N_DIMS; dim_i++)
            simplex[vertex_i][tri_order[dim_i]] += standard_simplex_[vertex_i][dim_i];
    }
}

template <typename index_t, int N_DIMS, int N_OPS>
int LinearCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::interpolate(const std::vector<double> &point,
                                                                   std::vector<double> &values)
{
    std::array<int, N_DIMS> hypercube;
    std::array<double, N_DIMS> scaled_point;
    this->find_hypercube(point, hypercube, scaled_point);

    std::array<std::array<int, N_DIMS>, N_DIMS + 1> simplex;
    std::array<int, N_DIMS> tri_order;
    find_simplex(hypercube, scaled_point, tri_order, simplex);

    std::array<double, N_DIMS + 1> weights;
    weights[0] = scaled_point[tri_order[0]];
    weights[N_DIMS] = 1.0 - scaled_point[tri_order[N_DIMS - 1]];

    for (int dim_i = 1; dim_i < N_DIMS; dim_i++)
        weights[dim_i] = scaled_point[tri_order[dim_i]] - scaled_point[tri_order[dim_i - 1]];
    values.assign(N_OPS, 0.0);
    for (int dim_i = 0; dim_i <= N_DIMS; dim_i++)
    {
        std::array<double, N_OPS> supp_values;
        this->get_supporting_point(simplex[dim_i], supp_values);
        for (int op = 0; op < N_OPS; op++)
            values[op] += weights[dim_i] * supp_values[op];
    }
    return 0;
}

template <typename index_t, int N_DIMS, int N_OPS>
int
LinearCPUInterpolatorBase<index_t, N_DIMS, N_OPS>::interpolate_with_derivatives(const std::vector<double> &points,
                                                                                const std::vector<int> &points_idxs, std::vector<double> &values, std::vector<double> &derivatives)
{
    values.resize(points_idxs.size() * N_OPS);
    derivatives.resize(points_idxs.size() * N_DIMS * N_OPS);

    for (std::size_t point_i = 0; point_i < points_idxs.size(); point_i++)
    {
        int point_offset = points_idxs[point_i];

        std::array<int, N_DIMS> hypercube;
        std::array<double, N_DIMS> scaled_point;
        this->find_hypercube(points, hypercube, scaled_point, point_offset * N_DIMS);

        std::array<std::array<int, N_DIMS>, N_DIMS + 1> simplex;
        std::array<int, N_DIMS> tri_order;
        find_simplex(hypercube, scaled_point, tri_order, simplex);

        std::array<std::array<double, N_OPS>, N_DIMS + 1> simplex_vertex_values;
        for (int i = 0; i <= N_DIMS; i++)
            this->get_supporting_point(simplex[i], simplex_vertex_values[i]);

        for (int i = 0; i < N_OPS; i++)
            values[point_offset * N_OPS + i] = simplex_vertex_values[0][i];
        double simplex_vertex_values_gap;
        for (int dim_i = 0; dim_i < N_DIMS; dim_i++)
        {
            for (int op_i = 0; op_i < N_OPS; op_i++)
            {
                simplex_vertex_values_gap = simplex_vertex_values[dim_i + 1][op_i] - simplex_vertex_values[dim_i][op_i];
                values[point_offset * N_OPS + op_i] += (1 - scaled_point[tri_order[dim_i]]) * simplex_vertex_values_gap;

                derivatives[(point_offset * N_OPS + op_i) * N_DIMS + tri_order[dim_i]] =
                        -simplex_vertex_values_gap * this->axes_step_inv_[tri_order[dim_i]];
            }
        }
    }
    return 0;
}
