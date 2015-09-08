PROJECT("Sand tracer")

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/.cmake")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Eigen REQUIRED)

FIND_PACKAGE(Boost 1.53.0 QUIET COMPONENTS system filesystem program_options unit_test_framework)
MESSAGE(STATUS "Boost ${Boost_LIB_VERSION} ?")
MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
MESSAGE(STATUS "Boost_DEFINITIONS = ${Boost_DEFINITIONS}")
IF(Boost_FOUND)
   MESSAGE(STATUS "Boost ${Boost_LIB_VERSION} found")
   include_directories(${Boost_INCLUDE_DIRS})
   link_directories(${Boost_LIBRARIES})
   add_definitions(${Boost_DEFINITIONS})
ELSE()
  MESSAGE(STATUS "Boost not found -- disabling the vocabulary tree")
ENDIF(Boost_FOUND)

INCLUDE_DIRECTORIES("include" ${Eigen_INCLUDE_DIR})

SET(SOURCES src/sandtracer.cpp src/geometry.cpp include/geometry.h)

ADD_EXECUTABLE(sandtracer ${SOURCES})
