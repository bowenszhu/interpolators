//*************************************************************************
//    Copyright (c) 2018
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

#include "py_globals.h"
#include "py_evaluator_iface.h"

namespace py = pybind11;

void pybind_evaluator_iface(py::module &m)
{
  using namespace pybind11::literals;

  py::class_<property_evaluator_iface, py_property_evaluator_iface /* <--- trampoline*/>(m, "property_evaluator_iface")
      .def(py::init<>())
      .def("evaluate", (double(property_evaluator_iface::*)(const std::vector<double> &)) & property_evaluator_iface::evaluate, "Evaluate single property value", "state"_a)
      .def("evaluate", (int (property_evaluator_iface::*)(const std::vector<double> &, int, std::vector<double> &)) & property_evaluator_iface::evaluate, "Evaluate property values for several states ", "states"_a, "n_blocks"_a, "values"_a);

  py::class_<operator_set_evaluator_iface, py_operator_set_evaluator_iface /* <--- trampoline*/> (m, "operator_set_evaluator_iface")
      .def(py::init<>())
      .def("evaluate", &operator_set_evaluator_iface::evaluate, "Evaluate operator values", "states"_a, "values"_a)
      .def("init_timer_node", &operator_set_evaluator_iface::init_timer_node,"Initialize timer", "timer_node"_a);

  py::class_<operator_set_gradient_evaluator_iface, operator_set_evaluator_iface>(m, "operator_set_gradient_evaluator_iface")
      .def("init", &operator_set_gradient_evaluator_iface::init, "Initialize interpolator")
      .def("evaluate_with_derivatives", &operator_set_gradient_evaluator_iface::evaluate_with_derivatives,
               "Evaluate operators and derivatives (v)", "state"_a, "block_idx"_a, "values"_a, "derivatives"_a);
}
