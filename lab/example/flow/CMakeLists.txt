cmake_minimum_required(VERSION 3.2)
project(example-flow)

add_executable(example-flow
        example-flow.cpp
        src/utils.cpp
        src/Flow.cpp
        src/InputReader.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(example-flow dipacus scabolic-optimization scabolic-core scabolic-energy)
else()
    add_dependencies(example-flow scabolic-optimization scabolic-core scabolic-energy)
endif()

target_include_directories(example-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(example-flow
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