from interpolators import LinearAdaptiveCPUInterpolator_l_d_1_2, operator_set_evaluator_iface, value_vector

#assert interpolators.__version__ == "0.0.1"


class custom_evaluator(operator_set_evaluator_iface):
    def evaluate(self, state, values):
        values[0] = state[0]
        values[1] = state[0] * 2
        return 0

# check if itor is creatable
fun = custom_evaluator()
itr = LinearAdaptiveCPUInterpolator_l_d_1_2(fun, [4],  [5], [8])
result = value_vector([0, 2])
itr.evaluate(value_vector([6.3]), result)
print (result)
