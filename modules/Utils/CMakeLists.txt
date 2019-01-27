cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-utils)

add_library(scabolic-utils SHARED
        src/Utils.cpp)

target_include_directories(scabolic-utils PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${GEOC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS})

install(TARGETS scabolic-utils
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})