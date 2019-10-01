cmake_minimum_required(VERSION 3.2)
project(input-reader)

if (${CREATE_SHARED_LIBRARIES})
    set(LIBRARY_ACTION SHARED)
else()
    set(LIBRARY_ACTION STATIC)
endif ()

add_library(input-reader ${LIBRARY_ACTION} src/InputReader.cpp src/utils.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(input-reader dipacus scabolic-core scabolic-energy scabolic-optimization)
else()
    add_dependencies(input-reader scabolic-core scabolic-energy scabolic-optimization)
endif ()


target_include_directories(input-reader PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${DGTAL_INCLUDE_DIRS})
target_link_libraries(input-reader
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        ${DGTAL_LIBRARIES})
