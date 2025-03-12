/**
 * @file   ScfTask.cpp
 *
 * @date   Mar 7, 2014
 * @author Thomas Dresselhaus
 * @copyright \n
 *  This file is part of the program Serenity.\n\n
 *  Serenity is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.\n\n
 *  Serenity is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.\n\n
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with Serenity.
 *  If not, see <http://www.gnu.org/licenses/>.\n
 */
/* Include Class Header*/
#include "ScfTask.h"
#include <chrono>
#include <iostream>

namespace Serenity {

void ScfTask::run() {
  Eigen::MatrixXd bigmatrix = Eigen::MatrixXd::Random(5000, 5000);
  Eigen::MatrixXd othermatrix = Eigen::MatrixXd::Random(bigmatrix.rows(), bigmatrix.cols());

  // start timer
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  Eigen::MatrixXd result = bigmatrix * othermatrix;
  // end timer
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  // print out time
  std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << " seconds" << std::endl;

  return;
}


} /* namespace Serenity */
