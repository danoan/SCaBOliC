cmake_minimum_required(VERSION 3.2)
project(test-scabolic)

add_executable(test-scabolic
        testScabolic.cpp
        src/Test/TestEnergyOptimization.cpp
        src/Test/PixelTestInstances.cpp
        src/Test/LinelTestInstances.cpp
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
        -lscabolic-utils
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
        -lscabolic-utils
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






add_executable(experiment-compareImproveProve
        compareImproveProbe.cpp)

add_dependencies(experiment-compareImproveProve scabolic-core scabolic-energy scabolic-optimization scabolic-utils)

target_compile_definitions(experiment-compareImproveProve PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(experiment-compareImproveProve PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(experiment-compareImproveProve
        -lscabolic-utils
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

set_target_properties(experiment-compareImproveProve PROPERTIES COMPILE_FLAGS -fext-numeric-literals)






add_executable(exp-greedy
        expGreedy.cpp
        src/Test/TestEnergyOptimization.cpp
        src/Utils/Utils.cpp)

add_dependencies(exp-greedy scabolic-core scabolic-energy scabolic-optimization scabolic-utils)

target_compile_definitions(exp-greedy PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(exp-greedy PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(exp-greedy
        -lscabolic-utils
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

set_target_properties(exp-greedy PROPERTIES COMPILE_FLAGS -fext-numeric-literals)












add_executable(exp-opening
        expOpening.cpp
        src/Test/TestEnergyOptimization.cpp
        src/Utils/Utils.cpp)

add_dependencies(exp-opening scabolic-core scabolic-energy scabolic-optimization scabolic-utils)

target_compile_definitions(exp-opening PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(exp-opening PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${CMAKE_SOURCE_DIR}/modules/Energy/include
        ${CMAKE_SOURCE_DIR}/modules/Utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(exp-opening
        -lscabolic-utils
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

set_target_properties(exp-opening PROPERTIES COMPILE_FLAGS -fext-numeric-literals)