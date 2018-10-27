cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-optimization)

add_library(scabolic-optimization SHARED
        src/qpbo/QPBO.cpp
        src/qpbo/QPBO_extra.cpp
        src/qpbo/QPBO_maxflow.cpp
        src/qpbo/QPBO_postprocessing.cpp
        src/solver/interface/IQPBOSolver.cpp
        src/solver/improve/QPBOImproveSolver.cpp
        src/solver/improveProbe/QPBOIP.cpp
        src/solver/probe/QPBOProbeSolver.cpp
        src/solver/simple/QPBOSimpleSolver.cpp)

target_include_directories(scabolic-optimization PRIVATE
        include)

install(TARGETS scabolic-optimization
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})