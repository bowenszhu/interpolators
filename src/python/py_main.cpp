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
#include "../timer_node.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;


void pybind_evaluator_iface(py::module &);
void pybind_operator_set_interpolator_pz(py::module &);


class ms_well;
class operator_set_gradient_evaluator_iface;

PYBIND11_MODULE(interpolators, m) {
  m.doc() = "A library of computationally efficient multidimensional multivariate interpolators based for CPU and GPU";
  //auto m1 = m.def_submodule("engines", "Collection of DARTS simulators based on OBL approach");
  py::bind_vector<std::vector<int>>(m, "index_vector", py::module_local(true), py::buffer_protocol());
  py::bind_vector<std::vector<double>>(m, "value_vector", py::module_local(true), py::buffer_protocol());

  py::class_<timer_node>(m, "timer_node", py::module_local(true), "Timers tree structure")
      .def(py::init<>())
      .def("start", &timer_node::start)
      .def("stop", &timer_node::stop)
      .def("get_timer", &timer_node::get_timer)
      .def("print", &timer_node::print)
      .def("reset_recursive", &timer_node::reset_recursive)
      //properties
      .def_readwrite("node", &timer_node::node);
  py::bind_map<std::map<std::string, timer_node>>(m, "timer_map", py::module_local(true));

  pybind_evaluator_iface(m);

  pybind_operator_set_interpolator_pz(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
