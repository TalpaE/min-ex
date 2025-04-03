# version 3.15 is the minium cmake version supported by scikit-build-core
cmake_minimum_required(VERSION 3.15)

project(serenity
        VERSION 1.6.2
        DESCRIPTION "Serenity: A subsystem quantum chemistry program."
	LANGUAGES CXX)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a "
                        "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
                        "remove CMakeCache.txt and CMakeFiles.")
endif()

# the cmake folder contains CMake files to locate some modules
set(CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#########################
## Buildtype and Flags ##
#########################

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type specified. Will build Release")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif(NOT CMAKE_BUILD_TYPE)


# Optional flags
set(SERENITY_MARCH "x86-64" CACHE STRING "Compile with -march=<FLAG>")
option(WERROR "Compile with warnings as errors" OFF)
option(GCC_PROFILE "Compile with profile flags" OFF)
option(GCC_COVERAGE "Compile with coverage flags" OFF)
if (GCC_COVERAGE)
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif()
option(SERENITY_USE_LAPLACE_MINIMAX "Do not include laplace-minimax by default" OFF)
option(SERENITY_DOWNLOAD_DEPENDENCIES "If OFF, will require the operator to provide all dependencies." ON)

# Get some architecture info
get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
set(ARCH_LIB_PATH "lib")
if ("${LIB64}" STREQUAL "TRUE")
  set(ARCH_LIB_PATH "lib64")
endif()


##########################################################
## Set up lists of directories that need to be included ##
##########################################################

# Set up lists of files
include(src/Files.cmake)

##################
## Main Targets ##
##################

add_library(serenity ${SERENITY_CPPS} ${SERENITY_HEADERS})
set_property(TARGET serenity PROPERTY POSITION_INDEPENDENT_CODE ON)
if (NOT DEFINED SKBUILD)
  add_executable(serenity_exe ${PROJECT_SOURCE_DIR}/src/serenity.cpp)
endif()

##################
## Dependencies ##
##################
message("#=========================#")
message("|  External Dependencies  |")
message("#=========================#")

# Eigen3
# Intel MKL
message("-----Checking Eigen3-----")
message("-----Checking Intel MKL-----")
include(AddEigen)
add_eigen(serenity PUBLIC)

# OpenMP
message("-----Checking OpenMP-----")
find_package(OpenMP REQUIRED)

# Laplace Minimax
if (SERENITY_USE_LAPLACE_MINIMAX)
  message("-----Checking Laplace_Minimax-----")
  if (NOT SERENITY_DOWNLOAD_DEPENDENCIES)
    message("Laplace-Minimax cannot be detected by CMake, it will be downloaded despite SERENITY_DOWNLOAD_DEPENDENCIES being OFF.")
  endif()
  include(ImportLaplaceMinimax)
  import_laplace_minimax()
endif(SERENITY_USE_LAPLACE_MINIMAX)

#################
## Definitions ##
#################
find_package(Git)
if(Git_FOUND)
  message(STATUS "Found git executable: ${GIT_EXECUTABLE}")
  # In case no git repository is found, only redirect stderr to /dev/null.
  execute_process ( COMMAND bash -c "git rev-parse --abbrev-ref HEAD 2> /dev/null" OUTPUT_VARIABLE GIT_BRANCH )
  if (GIT_BRANCH STREQUAL "")
    message(STATUS "No git repository found.")
    set(GIT_BRANCH "UNKNOWN")
  else ()
    # Remove new line character.
    string ( STRIP ${GIT_BRANCH} GIT_BRANCH )
    message(STATUS "Found git repository: " ${GIT_BRANCH})
  endif()
else ()
  set(GIT_BRANCH "UNKNOWN")
endif()
add_definitions (-DGIT_BRANCH="${GIT_BRANCH}")
# configure_file (${PROJECT_SOURCE_DIR}/src/io/FormattedOutput.cpp ${PROJECT_SOURCE_DIR}/src/io/FormattedOutput.cpp)

#################################
## Add dependencies to targets ##
#################################

# Shared library
target_include_directories(serenity PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/serenity>
  $<$<BOOL:${MKL_FOUND}>:${MKL_INCLUDE_DIRS}>
)
target_compile_options(serenity PRIVATE ${OpenMP_CXX_FLAGS})
set_target_properties(serenity PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  CXX_STANDARD 14
)
target_link_libraries(serenity
  PUBLIC
    OpenMP::OpenMP_CXX
    $<$<BOOL:${GCC_COVERAGE}>:-lgcov>
    $<$<BOOL:${GCC_PROFILE}>:-pg>
  PRIVATE
    $<$<BOOL:${SERENITY_USE_LAPLACE_MINIMAX}>:laplace-minimax>
)

target_compile_definitions(serenity PUBLIC
  $<$<BOOL:${SERENITY_USE_LAPLACE_MINIMAX}>:SERENITY_USE_LAPLACE_MINIMAX>
  $<$<CONFIG:Debug>:EIGEN_INITIALIZE_MATRICES_BY_NAN>
  $<$<BOOL:${MKL_FOUND}>:EIGEN_USE_MKL_ALL>
)
target_compile_options(serenity PUBLIC
  -Wall
  -Wextra
  -Wno-comment
  $<$<BOOL:${WERROR}>:-Werror>
  $<$<BOOL:${GCC_COVERAGE}>:-fprofile-arcs -ftest-coverage>
  $<$<BOOL:${GCC_PROFILE}>:-pg>
  $<$<AND:$<BOOL:${MKL_FOUND}>,$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","Intel">>:-DMKL_LP64>
)
if(NOT "${SERENITY_MARCH}" STREQUAL "" AND NOT MSVC)
  target_compile_options(serenity PUBLIC -march=${SERENITY_MARCH})
endif()


# Executable
if (NOT DEFINED SKBUILD)
  set_target_properties(serenity_exe PROPERTIES OUTPUT_NAME serenity)
  target_link_libraries(serenity_exe
    PUBLIC
      $<$<BOOL:${GCC_COVERAGE}>:-lgcov>
      $<$<BOOL:${GCC_PROFILE}>:-pg>
    PRIVATE
      serenity
  )
  set_target_properties(serenity_exe PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CXX_STANDARD 14
  )
  target_compile_options(serenity_exe
    PUBLIC
      -Wall
      -Wextra
      -Wno-comment
      $<$<BOOL:${WERROR}>:-Werror>
      $<$<BOOL:${GCC_COVERAGE}>:-fprofile-arcs -ftest-coverage>
      $<$<BOOL:${GCC_PROFILE}>:-pg>
  )
  if(NOT "${SERENITY_MARCH}" STREQUAL "" AND NOT MSVC)
    target_compile_options(serenity_exe PUBLIC -march=${SERENITY_MARCH})
  endif()
endif()

#############
## Install ##
#############

# Targets
install(TARGETS serenity
        EXPORT serenityTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
if (NOT DEFINED SKBUILD)
install(TARGETS serenity_exe
        EXPORT serenityTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
  DESTINATION include/serenity
  FILES_MATCHING PATTERN "*.h"
)
endif()

####################
## Python Wrapper ##
####################

option(SERENITY_PYTHON_BINDINGS "Enable Python interface" ON)
if (SERENITY_PYTHON_BINDINGS)
  message("#===============================#")
  message("| Python Interface Dependencies |")
  message("#===============================#")
  if(SERENITY_DOWNLOAD_DEPENDENCIES)
    include(ImportPybind11)
    import_pybind11()
  else()
    find_package(pybind11 REQUIRED)
  endif()
  # target
  #  set(PYBIND11_PYTHON_VERSION ${PYTHONVERSION})
  pybind11_add_module(serenipy
    ${PROJECT_SOURCE_DIR}/src/python/serenipy.cpp
    ${SERENITY_PYTHON_FILES}
  )
  set_target_properties(serenipy
    PROPERTIES
      SUFFIX ".so"
      CXX_STANDARD 14
      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  )
  target_link_libraries(serenipy
    PUBLIC
      $<$<BOOL:${GCC_COVERAGE}>:-lgcov>
      $<$<BOOL:${GCC_PROFILE}>:-pg>
    PRIVATE
      serenity
  )
  if(SERENITY_USAGE_FROM_SOURCE)
      add_custom_command(TARGET serenipy POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/lib/serenipy.so ${CMAKE_SOURCE_DIR}/qcserenity/serenipy.so)
  endif()
  target_compile_options(serenipy
    PUBLIC
      $<$<BOOL:${WERROR}>:-Werror>
      $<$<BOOL:${GCC_COVERAGE}>:-fprofile-arcs -ftest-coverage>
      $<$<BOOL:${GCC_PROFILE}>:-pg>
  )
  if(NOT "${SERENITY_MARCH}" STREQUAL "" AND NOT MSVC)
    target_compile_options(serenipy PUBLIC -march=${SERENITY_MARCH})
  endif()

  # Install
  install(TARGETS serenipy
    EXPORT serenityTargets
    LIBRARY DESTINATION qcserenity
    )
endif(SERENITY_PYTHON_BINDINGS)

#################################
## Allow usage without install ##
#################################

option(SERENITY_USAGE_FROM_SOURCE "Generate files for easy usage without invoking 'make install'." ON)
if(SERENITY_USAGE_FROM_SOURCE)
  # Create links to bin, lib and include
  add_custom_command(TARGET serenity POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}/lib" "${CMAKE_CURRENT_SOURCE_DIR}/lib")
  add_custom_command(TARGET serenity POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}/bin" "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif(SERENITY_USAGE_FROM_SOURCE)

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE Serenity)

# Install the export set for use with the install-tree
install(EXPORT serenityTargets
  FILE serenity-targets.cmake
  DESTINATION lib/cmake/serenity
)
