cmake_minimum_required(VERSION 3.2)
project(test-scabolic)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(test-scabolic src/testScabolic.cpp)

target_include_directories(test-scabolic PRIVATE
        ${DGTAL_INCLUDE_DIRS}
        ../ext/include
        include
        ../modules/Optimization/include
        ../modules/Core/include
        ../modules/Energy/include
        ../modules/API/include
        )

target_link_libraries(test-scabolic
        -lscabolic-api
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS}
        -lboost_system
        -lboost_filesystem)

set_target_properties(test-scabolic PROPERTIES COMPILE_FLAGS -fext-numeric-literals)
