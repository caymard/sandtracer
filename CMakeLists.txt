PROJECT("Sand tracer")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

INCLUDE_DIRECTORIES("include")

SET(SOURCES src/sandtracer.cpp src/geometry.cpp include/geometry.h)

ADD_EXECUTABLE(sandtracer ${SOURCES})