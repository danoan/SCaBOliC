cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-utils)

set(LIBRARY_ACTION "STATIC")
if(${CREATE_SHARED_LIBRARIES})
    set(LIBRARY_ACTION "SHARED")
endif()

add_library(scabolic-utils ${LIBRARY_ACTION}
        src/Utils.cpp)

if(${USE_LOCAL_REPOSITORIES})
    add_dependencies(scabolic-utils dipacus geoc)
endif()

target_include_directories(scabolic-utils PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${GEOC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS})

install(TARGETS scabolic-utils
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})