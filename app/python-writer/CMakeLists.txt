cmake_minimum_required(VERSION 3.2)
project(python-writer)

add_executable(python-writer python-writer.cpp)

add_dependencies(python-writer scabolic-core scabolic-energy scabolic-optimization)

target_include_directories(python-writer PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/app/input-reader/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(python-writer
        -linput-reader
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lscabolic-optimization
        -lscabolic-energy
        -lboost_filesystem
        -lboost_system
        ${GUROBI_HOME}/lib/libgurobi_c++.a
        ${GUROBI_HOME}/lib/libgurobi81.so
        ${OpenCV_LIBS}
        ${DGTAL_LIBRARIES})