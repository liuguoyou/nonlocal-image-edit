cmake_minimum_required(VERSION 2.8)

project(nonlocal)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

option(USE_BLAS "Use BLAS and LAPACK" OFF)
if (USE_BLAS)
    find_package(BLAS REQUIRED)
    include_directories( ${BLAS_INCLUDE_DIRS} )
    find_package(LAPACK REQUIRED)
    include_directories( ${LAPACK_INCLUDE_DIRS} )
    add_definitions(-DEIGEN_USE_BLAS -DEIGEN_USE_LAPACK)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ext)

option(USE_SPECTRA "Use SPECTRA for eigendecomposition" OFF)
if (USE_SPECTRA)
    message(STATUS "Using SPECTRA for eigen decomposition.")
    add_definitions(-DUSE_SPECTRA)
endif()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# option(USE_OPENMP "Use OpenMP" OFF)
# if (USE_OPENMP)
#     find_package(OpenMP)
#     # if(OpenMP_CXX_FOUND)
#     #     target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
#     # endif()
# endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release mode")
    add_compile_options(-march=native)
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode")
    add_compile_options(-ggdb3)
endif()

file(GLOB PROJECT_SRC
    "*.hpp"
    "*.cpp"
)

add_executable("enhance" "src/enhance.cpp" "src/filter.cpp" "include/filter.hpp" "include/utils.hpp")
add_executable("denoise" "src/denoise.cpp" "src/filter.cpp" "include/filter.hpp" "include/utils.hpp")

# Prepare "Catch" library for other executables

# add_subdirectory(ext/)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test_filter.cpp)
add_executable(tests ${TEST_SOURCES} "src/filter.cpp" "include/filter.hpp" "include/utils.hpp")
target_link_libraries(tests Catch ${OpenCV_LIBS} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} Eigen3::Eigen)


# add_executable(${PROJECT_NAME} ${PROJECT_SRC} "include/filter.hpp")
add_compile_options(-Wall -Wextra -Wpedantic)
# target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} OpenMP::OpenMP_CXX ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} Eigen3::Eigen)
# target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} Eigen3::Eigen)
target_link_libraries("enhance" ${OpenCV_LIBS} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} Eigen3::Eigen)
target_link_libraries("denoise" ${OpenCV_LIBS} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} Eigen3::Eigen)
