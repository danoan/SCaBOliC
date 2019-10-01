cmake_minimum_required(VERSION 3.2)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "SCaBOliC binaries and libraries install location." )
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${BOOST_LIBS_DIR}:${DIPACUS_LIBS_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(EXTPROJECTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext-projects/source)
if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-release)
else()
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-debug)
endif ()

set(BOOST_INCLUDE_DIRS /usr/local/include/boost CACHE PATH "Boost include location path." )
set(BOOST_LIBS_DIR /usr/local/lib CACHE PATH "Boost libraries location path." )

set(EIGEN_INCLUDE_DIRS /usr/local/include/eigen3 CACHE PATH "Eigen include location path." )

set(DIPACUS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "geoc include location path.")

set(BUILD_TESTS OFF CACHE BOOL "Build tests.")
set(CREATE_SHARED_LIBRARIES OFF CACHE BOOL "Create shared libraries. If off, create static libraries.")
set(USE_REMOTE_REPOSITORIES ON CACHE BOOL "Pull external repositories from GitHub.")

project(SCaBOliC)
set(VERSION_SCABOLIC_CORE master)
set(VERSION_SCABOLIC_ENERGY master)
set(VERSION_SCABOLIC_OPTIMIZATION master)
set(VERSION_SCABOLIC_UTILS master)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

message("Project Root Dir: ${CMAKE_SOURCE_DIR}")
message("Build files at: ${CMAKE_BINARY_DIR}")
message("Installation at: ${CMAKE_INSTALL_PREFIX}")

if(${USE_REMOTE_REPOSITORIES})
    include(ext-projects/ExtProjects.cmake)
endif()

link_directories(${CMAKE_BINARY_DIR}/modules/Optimization
        ${CMAKE_BINARY_DIR}/modules/Core
        ${CMAKE_BINARY_DIR}/modules/Energy
        ${CMAKE_BINARY_DIR}/modules/Utils
        ${CMAKE_BINARY_DIR}/app/input-reader
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIBS_DIR})

add_subdirectory(modules/Core)
add_subdirectory(modules/Optimization)
add_subdirectory(modules/Energy)
add_subdirectory(modules/Utils)

if(${BUILD_TESTS})
    add_subdirectory(lab/example)
endif()

add_subdirectory(app)
