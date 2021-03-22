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
#ifndef PY_GLOBALS_H
#define PY_GLOBALS_H

#include <pybind11/stl_bind.h>

namespace py = pybind11;
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);

#define MAX_NC 4

// exposer helper class for <N_DiMS, N_OPS> template: N_OPS=N_DIMS*N_OPS_A + N_OPS_B
template <template <uint8_t N_DIMS, uint8_t N_OPS> class exposer_t, typename pymodule_t, uint8_t N_DIMS, uint8_t N_OPS_A, uint8_t N_OPS_B>
struct recursive_exposer_ndims_nops
{
  static void expose(pymodule_t &m)
  {
    exposer_t<N_DIMS, N_DIMS * N_OPS_A + N_OPS_B> e;

    e.expose(m);

    recursive_exposer_ndims_nops<exposer_t, pymodule_t, N_DIMS - 1, N_OPS_A, N_OPS_B>::expose(m);
  }
};

// partial specialization to stop recusrion

template <template <uint8_t N_DIMS, uint8_t N_OPS> class exposer_t, typename pymodule_t, uint8_t N_OPS_A, uint8_t N_OPS_B>
struct recursive_exposer_ndims_nops<exposer_t, pymodule_t, 1, N_OPS_A, N_OPS_B>
{
  static void expose(pymodule_t &m)
  {
    exposer_t<1, 1 * N_OPS_A + N_OPS_B> e;

    e.expose(m);
  }
};


#endif





