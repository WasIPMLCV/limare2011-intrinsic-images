cmake_minimum_required (VERSION 2.6)

project (retinex_pde)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(LIBM REQUIRED)
find_package(PNG REQUIRED)
find_package(FFTW REQUIRED)

include_directories(${PNG_INCLUDE_DIR} ${FFTW_INCLUDE_DIR} ${LIBM_INCLUDE_DIR})

add_library(retinex_pde retinex_pde_lib.c)
target_link_libraries(retinex_pde ${FFTW_LIBRARIES} ${LIBM_LIBRARIES})

add_library(io_png io_png.c)
target_link_libraries(io_png ${PNG_LIBRARIES})

add_executable(retinex_pde_cli retinex_pde.c norm.c)
target_link_libraries(retinex_pde_cli io_png retinex_pde)
