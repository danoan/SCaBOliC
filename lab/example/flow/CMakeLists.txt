cmake_minimum_required(VERSION 3.2)
project(example-flow)

add_executable(example-flow
        src/Flow.cpp
        example-flow.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(example-flow dipacus scabolic-optimization scabolic-core scabolic-energy input-reader)
else()
    add_dependencies(example-flow scabolic-optimization scabolic-core scabolic-energy input-reader)
endif()

target_include_directories(example-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${CMAKE_SOURCE_DIR}/app/input-reader/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(example-flow
        -linput-reader
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lscabolic-utils
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})