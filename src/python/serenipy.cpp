/**
 * @file serenipy.cpp
 *
 * @date: Apr 25, 2016
 * @author: Jan Unsleber
 * @copyright \n
 *  This file is part of the program Serenity.\n\n
 *  Serenity is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.\n\n
 *  Serenity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.\n\n
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with Serenity.
 *  If not, see <http://www.gnu.org/licenses/>.\n
 */

#include <omp.h>
#include <pybind11/iostream.h> // Include to allow ostream redirect
#include <pybind11/pybind11.h>
#include <iostream>
#include <memory>
namespace py = pybind11;

/*
 * Forward declare everything
 */
void export_ScfTask(py::module& spy);

PYBIND11_MODULE(serenipy, spy) {
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  spy.doc() = "Serenipy - Python Bindings for Serenity ";

  export_ScfTask(spy);
}
