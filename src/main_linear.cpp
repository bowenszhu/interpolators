#include <iostream>
#include "evaluator_iface.h"
#include "linear_static_cpu_interpolator.hpp"

class op : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state, std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0)
                    + state[0] * state[0] + state[1] * state[1] * state[1];
        return 0;
    }
};

int main() {
    const int n_dim = 2;
    std::vector<int> axes_points{3, 4};
    std::vector<double> axes_min{3.1, 4.2};
    std::vector<double> axes_max{5.1, 6.2};
    op iface = op();
    linear_static_cpu_interpolator<int, n_dim, 1> linear_interpolator(&iface, axes_points, axes_min, axes_max);

    std::vector<double> point {4.1, 6.1};
    std::vector<double> values (n_dim);
    if (!linear_interpolator.init() && !linear_interpolator.interpolate(point, values))
        std::cout << "value = " << values[0] << std::endl;
    else
        std::cout << "linear interpolation failed" << std::endl;

    return 0;
}
