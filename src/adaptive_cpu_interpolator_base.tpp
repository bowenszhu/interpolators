#include <cmath>
#include <algorithm>
#include "adaptive_cpu_interpolator_base.hpp"


template<typename index_t, int N_DIMS, int N_OPS>
adaptive_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::adaptive_cpu_interpolator_base(
        operator_set_evaluator_iface *supportingPointEvaluator, const std::array<int, N_DIMS> &axesPoints,
        const std::array<double, N_DIMS> &axesMin, const std::array<double, N_DIMS> &axesMax)
        : interpolator_base<index_t, N_DIMS, N_OPS>(supportingPointEvaluator, axesPoints, axesMin, axesMax) {}

template <typename index_t, int N_DIMS, int N_OPS>
void adaptive_cpu_interpolator_base<index_t, N_DIMS, N_OPS>::get_supporting_point(const std::array<int, N_DIMS> &vertex,
                                                                                  std::array<double, N_OPS> &values)
{
    index_t index = this->get_index_from_vertex(vertex);
    auto search = point_data.find(index);
    if (search == point_data.end()) ///< std::unordered_map<...>::contains is supported since C++20
    {
//        this->timer->node["point generation"].start();
        std::vector<double> new_point_coords (N_DIMS);
        this->get_point_from_vertex(vertex, new_point_coords);

        std::vector<double> new_operator_values (N_OPS);
        this->supporting_point_evaluator->evaluate(new_point_coords, new_operator_values);

        std::copy(new_operator_values.begin(), new_operator_values.end(), point_data[index].begin());
        std::copy(new_operator_values.begin(), new_operator_values.end(), values.begin());

        for (int j = 0; j < N_OPS; j++)
        {
            if (std::isnan(new_operator_values[j]))
            {
                printf("OBL generation warning: NaN operator detected! Operator %d for point (", j);
                for (int a = 0; a < N_DIMS; a++)
                    printf("%lf, ", new_point_coords[a]);
                printf(") is %lf\n", new_operator_values[j]);
            }
        }
//        this->timer->node["point generation"].stop();
        this->n_points_used++;
    } else {
        std::copy(search->second.begin(), search->second.end(), values.begin());
    }
}