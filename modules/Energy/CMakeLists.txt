cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-energy)

set(LIBRARY_ACTION "STATIC")
if(${CREATE_SHARED_LIBRARIES})
    set(LIBRARY_ACTION "SHARED")
endif()

add_library(scabolic-energy ${LIBRARY_ACTION}
        src/Energy/ISQ/Terms/SquaredCurvature/CoefficientComputer.cpp
        src/Energy/ISQ/VariableMap.cpp
        src/Energy/ISQ/Terms/Data/DataTerm.cpp
        src/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.cpp
        src/Energy/ISQ/Terms/Length/LengthTerm.cpp
        src/Energy/base/EnergyTerm.cpp
        src/Energy/ISQ/ISQEnergy.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(scabolic-energy dipacus geoc scabolic-core scabolic-optimization scabolic-utils)
else()
    add_dependencies(scabolic-energy scabolic-core scabolic-optimization scabolic-utils)
endif()


target_include_directories(scabolic-energy PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/Optimization/include
        ${CMAKE_SOURCE_DIR}/modules/Core/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${EIGEN_INCLUDE_DIRS}
        )

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
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

