cmake_minimum_required(VERSION 3.2)
project(test-scabolic)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(test-scabolic
        testScabolic.cpp
        src/Test/TestEnergyOptimization.cpp
        src/Utils/Utils.cpp)

target_compile_definitions(test-scabolic PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(test-scabolic PRIVATE
        include
        ../ext/include
        ../modules/Optimization/include
        ../modules/Core/include
        ../modules/Energy/include
        ../modules/Utils/include
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
        src/Experiment/ExpFlow.cpp
        src/Utils/Utils.cpp
        src/Test/TestEnergyOptimization.cpp)

target_compile_definitions(experiment-scabolic PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(experiment-scabolic PRIVATE
        include
        ../ext/include
        ../modules/Optimization/include
        ../modules/Core/include
        ../modules/Energy/include
        ../modules/Utils/include
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

