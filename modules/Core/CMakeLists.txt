cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-core)

add_library(scabolic-core SHARED
        src/display.cpp
        src/ODRPixels.cpp
        src/ODRInterpixels.cpp
        src/ODRUtils.cpp
        src/PixelSpaceHandle.cpp
        src/InterpixelSpaceHandle.cpp)

target_include_directories(scabolic-core PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(scabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES})

install(TARGETS scabolic-core
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})