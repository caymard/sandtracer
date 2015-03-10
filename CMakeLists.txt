PROJECT("Sand tracer")

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/.cmake")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Eigen REQUIRED)

INCLUDE_DIRECTORIES("include" ${Eigen_INCLUDE_DIR}) 

SET(SOURCES src/sandtracer.cpp src/geometry.cpp include/geometry.h)

ADD_EXECUTABLE(sandtracer ${SOURCES})
