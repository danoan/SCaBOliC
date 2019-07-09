cmake_minimum_required(VERSION 3.0)
project(SCaBOliC)

#CMAKE_INSTALL_PREFIX: install location
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

message("Project Root Dir: ${CMAKE_SOURCE_DIR}")
message("Build files at: ${CMAKE_BINARY_DIR}")
message("Installation at: ${CMAKE_INSTALL_PREFIX}")

set(BOOST_INCLUDE_DIRS /usr/local/include/boost CACHE PATH "Boost include location path." )
set(BOOST_LIBS_DIR /usr/local/lib CACHE PATH "Boost libraries location path." )

set(EIGEN_INCLUDE_DIRS /usr/local/include/eigen3 CACHE PATH "Eigen include location path." )

set(DIPACUS_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ext/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR /usr/local/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS /usr/local/include/geoc CACHE PATH "geoc include location path.")

set(BUILD_TESTS OFF CACHE BOOL "Build tests.")
set(CREATE_SHARED_LIBRARIES OFF CACHE BOOL "Create shared libraries. If off, create static libraries.")

link_directories(${CMAKE_BINARY_DIR}/modules/Optimization
        ${CMAKE_BINARY_DIR}/modules/Core
        ${CMAKE_BINARY_DIR}/modules/Energy
        ${CMAKE_BINARY_DIR}/modules/Utils
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

add_subdirectory(modules/Core)
add_subdirectory(modules/Optimization)
add_subdirectory(modules/Energy)
add_subdirectory(modules/Utils)

if(${BUILD_TESTS})
    add_subdirectory(modules/lab/test/flow)
endif()
