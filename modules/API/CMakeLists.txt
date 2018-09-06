cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-api)

find_package(DGtal REQUIRED)

add_library(scabolic-api SHARED src/api.cpp)

target_include_directories(scabolic-api PRIVATE
        ${DGTAL_INCLUDE_DIRS}
        include
        ../Optimization/include
        ../Core/include
        ../Energy/include
        ../../ext/include
        )

target_link_libraries(scabolic-api
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES})

install(DIRECTORY
        include
        DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS scabolic-api
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)