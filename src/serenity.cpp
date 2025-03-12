/**
 * @file serenity.cpp
 * @version 1.6.2
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

/* Include Serenity Internal Headers */
#include "ScfTask.h"
/* Include Std and External Headers */
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using namespace Serenity;
/**
 * @brief The main function of the program.
 * @param argc
 * @param argv
 * @return
 */
int main(int , char* []) {
  // remove buffering on output
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  // create a ScfTask object
  ScfTask scf_task;
  scf_task.run();  

  printf("All clean and shiny");

  return 0;
}
