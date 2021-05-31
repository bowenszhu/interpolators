import interpolators

assert interpolators.__version__ == "0.0.1"

from interpolators import linear_adaptive_cpu_interpolator_l_d_1_2, operator_set_evaluator_iface, index_vector, value_vector


class custom_evaluator(operator_set_evaluator_iface):
    def evaluate(self, state, values):
        values[0] = state[0]
        return 0

# check if itor is creatable
fun = custom_evaluator()
itr = linear_adaptive_cpu_interpolator_l_d_1_2(fun, [4],  [5], [8])
