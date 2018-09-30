cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-energy)

add_library(scabolic-energy SHARED
        src/Energy/ISQ/base/EnergyTerm.cpp
        src/Energy/ISQ/Terms/Data/DataTerm.cpp
        src/Energy/ISQ/Terms/Data/LengthTerm.cpp
        src/Energy/ISQ/Terms/SquaredCurvature/CoefficientComputer.cpp
        src/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.cpp
        src/Energy/ISQ/ISQEnergy.cpp
        src/Energy/ISQ/VariableMap.cpp)

add_dependencies(scabolic-energy scabolic-core scabolic-optimization scabolic-utils)

target_include_directories(scabolic-energy PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${DGTAL_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS})

target_link_libraries(scabolic-energy
        -lscabolic-core
        -lscabolic-optimization
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS scabolic-energy
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

