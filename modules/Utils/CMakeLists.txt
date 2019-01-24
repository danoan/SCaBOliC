cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-utils)

find_package(DGtal REQUIRED)

add_library(scabolic-utils SHARED
        src/Utils.cpp)

target_include_directories(scabolic-utils PRIVATE
        include
        ${DGTAL_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS})

install(TARGETS scabolic-utils
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})