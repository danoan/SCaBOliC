cmake_minimum_required(VERSION 3.2)
project(example-flow)

add_library(example-lib-flow src/Flow.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(example-lib-flow dipacus scabolic-optimization scabolic-core scabolic-energy)
else()
    add_dependencies(example-lib-flow scabolic-optimization scabolic-core scabolic-energy)
endif()

target_include_directories(example-lib-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${CMAKE_SOURCE_DIR}/lab/example/input/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(example-lib-flow
        -lexample-input
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


add_executable(example-flow example-flow.cpp)
add_dependencies(example-flow example-lib-flow)

target_include_directories(example-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${CMAKE_SOURCE_DIR}/lab/example/input/include)

target_link_libraries(example-flow
        -lexample-lib-input
        -lexample-lib-flow
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