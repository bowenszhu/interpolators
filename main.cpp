#include <iostream>
#include <delaunay_adaptive_cpu_interpolator.h>
#include <delaunay_static_cpu_interpolator.h>
#include <linear_static_cpu_interpolator.h>
#include <linear_adaptive_cpu_interpolator.h>

class SampleOperators : public operator_set_evaluator_iface {
public:
    int evaluate(const std::vector<double> &state,
                 std::vector<double> &values) override {
        values[0] = std::accumulate(state.begin(), state.end(), 0.0);
        values[1] = std::accumulate(state.begin(), state.end(), 1.0,
                                    std::multiplies<double>());
        return 0;
    }
};

int main() {
    const int n_dim = 8;
    const int n_op = 2;
    std::array<int, n_dim> axes_points;
    std::array<double, n_dim> axes_min;
    std::array<double, n_dim> axes_max;
    axes_points.fill(20);
    axes_min.fill(0.0);
    axes_max.fill(1.0);
    SampleOperators iface = SampleOperators();
    std::map<std::string, operator_set_gradient_evaluator_iface*> itors;
    std::cout << "Creating Linear..." << std::endl << std::flush;
    itors["Linear Adaptive"] = new LinearAdaptiveCPUInterpolator<uint64_t, n_dim, n_op>(&iface, axes_points, axes_min, axes_max);
    std::cout << "Creating Delaunay..." << std::endl << std::flush;

    itors["Delaunay Adaptive"] = new DelaunayAdaptiveCPUInterpolator<uint64_t, n_dim, n_op>(&iface, axes_points, axes_min, axes_max);

    std::vector<double> point;
    std::vector<int> points_idxs{0};
    std::vector<double> values;
    std::vector<double> derivatives;
    point.assign(n_dim, 0.34);
    values.resize(n_op);
    derivatives.resize(n_op * n_dim);
    for (auto const& [name, interpolator] : itors) {
        if (!interpolator->evaluate_with_derivatives(point, points_idxs,
                                                    values, derivatives))
        {
            std::cout << "Testing " << name << ":" << std::endl
                    << "value      [0] = " << values[0] << std::endl
                    << "value      [1] = " << values[1] << std::endl
                    << "derivatives[0] = " << derivatives[0] << std::endl
                    << "derivatives[1] = " << derivatives[1] << std::endl
                    << "derivatives[2] = " << derivatives[2] << std::endl
                    << "derivatives[3] = " << derivatives[3] << std::endl;
            std::string timer_stat;
            interpolator->timer->print("", timer_stat);
            std::cout << timer_stat << std::flush;
        } else
            std::cout << name << " failed" << std::endl;
    }

    return 0;
}