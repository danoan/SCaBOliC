cmake_minimum_required(VERSION 3.2)
project(example-input)

add_library(example-lib-input src/InputReader.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(example-lib-input dipacus scabolic-optimization scabolic-core scabolic-energy)
else()
    add_dependencies(example-lib-input scabolic-optimization scabolic-core scabolic-energy)
endif()

target_include_directories(example-lib-input PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(example-lib-input
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