/**
 * @file   ScfTask.h
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
#ifndef SCFTASK_H_
#define SCFTASK_H_
/* Include Std and External Headers */
#include <memory> //smart ptr.
#include <Eigen/Dense> //linear algebra.

namespace Serenity {

/**
 * @class ScfTask ScfTask.h
 * @brief Performs an SCF calculation, i.e. an electronic structure calculation.
 */
class ScfTask {
 public:
  void run();
};

} /* namespace Serenity */

#endif /* SCFTASK_H_ */
