cmake_minimum_required(VERSION 3.2)
project(test-flow)

add_executable(test-flow test-flow.cpp)
add_dependencies(test-flow scabolic-optimization scabolic-core scabolic-energy)
target_include_directories(test-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-flow
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lscabolic-utils
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})