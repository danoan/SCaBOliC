cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-regularization)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_library(scabolic-regularization SHARED src/SQCurv/Model/CoeficientsComputer.cpp )

target_include_directories(scabolic-regularization PRIVATE
        ${DGTAL_INCLUDE_DIRS}
        include
        ../Optimization/include
        ../Core/include
        ../../ext/include)

target_link_libraries(scabolic-regularization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS scabolic-regularization
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

