cmake_minimum_required(VERSION 3.2)
project(levels-contribution)

add_executable(levels-contribution levels-contribution.cpp src/PotentialMap.cpp include/PotentialMap.h)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(levels-contribution dipacus scabolic-optimization scabolic-core scabolic-energy example-lib-input example-lib-flow)
else()
    add_dependencies(levels-contribution scabolic-optimization scabolic-core scabolic-energy example-lib-input example-lib-flow)
endif()


target_include_directories(levels-contribution PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/lab/example/input/include
        ${CMAKE_SOURCE_DIR}/lab/example/flow/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(levels-contribution
        -lexample-lib-flow
        -lexample-lib-input
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

INSTALL(TARGETS levels-contribution
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)