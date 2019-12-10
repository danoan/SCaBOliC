cmake_minimum_required(VERSION 3.12)
project(potential-set)

add_executable(potential-set potential-set.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(potential-set dipacus scabolic-core scabolic-energy)
else()
    add_dependencies(potential-set scabolic-core scabolic-energy)
endif()

target_include_directories(potential-set PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${DGTAL_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS})

target_link_libraries(potential-set
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lscabolic-energy
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})