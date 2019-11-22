cmake_minimum_required(VERSION 3.10)
project(unconstrained-model)

add_executable(unconstrained-model unconstrained-model.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(unconstrained-model dipacus)
endif()

set_target_properties(unconstrained-model PROPERTIES COMPILE_FLAGS "-fPIC")

target_include_directories(unconstrained-model PRIVATE
        include
        ${LBFGS_INCLUDE_DIRS}
        ${EIGEN_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})


target_link_libraries(unconstrained-model
        -ladept
        -ldipacus-base
        -ldipacus-components
        -ldipacus-derivates
        ${DGTAL_LIBRARIES})