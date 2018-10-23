cmake_minimum_required(VERSION 3.2)
project(test-scabolic)

add_executable(test-scabolic
        testScabolic.cpp
        src/Test/TestEnergyOptimization.cpp
        src/Test/TestInstances.cpp
        src/Test/TestEnergyEvaluation.cpp
        src/Utils/Utils.cpp)

add_dependencies(test-scabolic scabolic-core scabolic-energy scabolic-optimization scabolic-utils)

target_compile_definitions(test-scabolic PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(test-scabolic PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(test-scabolic
        -lscabolic-energy
        -lscabolic-core
        -lscabolic-optimization
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS}
        -lboost_system
        -lboost_filesystem)

set_target_properties(test-scabolic PROPERTIES COMPILE_FLAGS -fext-numeric-literals)





add_executable(experiment-scabolic
        expScabolic.cpp
        src/Experiment/ExpQPBOSolverType.cpp
        src/Experiment/ExpApplicationType.cpp
        src/Experiment/ExpFlowFromImage.cpp
        src/Utils/Utils.cpp
        src/Test/TestEnergyOptimization.cpp)

add_dependencies(experiment-scabolic scabolic-core scabolic-energy scabolic-optimization scabolic-utils)

target_compile_definitions(experiment-scabolic PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(experiment-scabolic PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(experiment-scabolic
        -lscabolic-energy
        -lscabolic-core
        -lscabolic-optimization
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS}
        -lboost_system
        -lboost_filesystem)

set_target_properties(experiment-scabolic PROPERTIES COMPILE_FLAGS -fext-numeric-literals)

