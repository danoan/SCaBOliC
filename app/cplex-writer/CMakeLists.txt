cmake_minimum_required(VERSION 3.2)
project(cplex-writer)

add_executable(cplex-writer cplex-writer.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(cplex-writer dipacus scabolic-core scabolic-energy scabolic-optimization input-reader)
else()
    add_dependencies(cplex-writer scabolic-core scabolic-energy scabolic-optimization input-reader)
endif ()


target_include_directories(cplex-writer PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/app/input-reader/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(cplex-writer
        -linput-reader
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lscabolic-optimization
        -lscabolic-energy
        -lboost_filesystem
        -lboost_system
        ${OpenCV_LIBS}
        ${DGTAL_LIBRARIES})