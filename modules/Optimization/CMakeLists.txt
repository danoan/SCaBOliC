cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-optimization)

set(LIBRARY_ACTION "STATIC")
if(${CREATE_SHARED_LIBRARIES})
    set(LIBRARY_ACTION "SHARED")
endif()

add_library(scabolic-optimization ${LIBRARY_ACTION}
        src/qpbo/QPBO.cpp
        src/qpbo/QPBO_extra.cpp
        src/qpbo/QPBO_maxflow.cpp
        src/qpbo/QPBO_postprocessing.cpp)

target_include_directories(scabolic-optimization PRIVATE
        include)

set_target_properties(scabolic-optimization PROPERTIES
        VERSION ${VERSION_SCABOLIC_OPTIMIZATION})

install(TARGETS scabolic-optimization
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})