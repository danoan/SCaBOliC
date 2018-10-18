cmake_minimum_required(VERSION 3.0)
project(SCaBOliC)

#CMAKE_INSTALL_PREFIX: install location

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

message("Project Root Dir: ${CMAKE_SOURCE_DIR}")
message("Build files at: ${CMAKE_BINARY_DIR}")
message("Installation at: ${CMAKE_INSTALL_PREFIX}")

set(BOOST_INCLUDE_DIRS /usr/local/include/boost CACHE PATH "Boost include location path." )
set(BOOST_LIB_DIR /usr/local/lib CACHE PATH "Boost libraries location path." )

set(DIPACUS_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ext/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR /usr/local/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS /usr/local/include/geoc CACHE PATH "geoc include location path.")

set(BUILD_TESTS OFF CACHE BOOL "Build tests.")

link_directories(${CMAKE_BINARY_DIR}/modules/Optimization
        ${CMAKE_BINARY_DIR}/modules/Core
        ${CMAKE_BINARY_DIR}/modules/Energy
        ${DIPACUS_LIBS_DIR}
        ${BOOST_LIB_DIR})

add_subdirectory(modules/Core)
add_subdirectory(modules/Optimization)
add_subdirectory(modules/Energy)
add_subdirectory(modules/Utils)

if(${BUILD_TESTS})
    add_subdirectory(lab)
endif()
