#include <iostream>
#include "evaluator_iface.h"
#include "linear_static_cpu_interpolator.hpp"
#include "linear_adaptive_cpu_interpolator.hpp"

class op : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state, std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0);
        values[1] = std::accumulate(state.begin(), state.end(), 1.0, std::multiplies<double>());
        return 0;
    }
};

int main() {
    const int n_dim = 2;
    std::array<int, n_dim> axes_points{3, 4};
    std::array<double, n_dim> axes_min{3.1, 4.2};
    std::array<double, n_dim> axes_max{5.1, 6.2};
    op iface = op();
    linear_adaptive_cpu_interpolator<int, n_dim, 2> linear_interpolator(&iface, axes_points, axes_min, axes_max);

    std::vector<double> point {4.1, 6.1};
    std::vector<int> points_idxs {0};
    std::vector<double> values (n_dim);
    std::vector<double> derivatives {};
    if (!linear_interpolator.interpolate_with_derivatives(point, points_idxs, values, derivatives))
    {
        std::cout << "value      [0] = " << values[0]      << std::endl;
        std::cout << "value      [1] = " << values[1]      << std::endl;
        std::cout << "derivatives[0] = " << derivatives[0] << std::endl;
        std::cout << "derivatives[1] = " << derivatives[1] << std::endl;
        std::cout << "derivatives[2] = " << derivatives[2] << std::endl;
        std::cout << "derivatives[3] = " << derivatives[3] << std::endl;
    }
    else
        std::cout << "linear interpolation failed" << std::endl;

    return 0;
}
