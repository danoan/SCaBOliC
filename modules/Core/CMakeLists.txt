cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-core)

set(LIBRARY_ACTION "STATIC")
if(${CREATE_SHARED_LIBRARIES})
    set(LIBRARY_ACTION "SHARED")
endif()

add_library(scabolic-core ${LIBRARY_ACTION}
        src/display.cpp
        src/ODRPixels.cpp
        src/ODRUtils.cpp
        src/PixelSpaceHandle.cpp)

if(${USE_LOCAL_REPOSITORIES})
    add_dependencies(scabolic-core dipacus)
endif()

target_include_directories(scabolic-core PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS})

target_link_libraries(scabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES})

install(TARGETS scabolic-core
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})