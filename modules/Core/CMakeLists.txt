cmake_minimum_required(VERSION 3.2)
project(lib-scabolic-core)

find_package(DGtal REQUIRED)

add_library(scabolic-core SHARED src/display.cpp src/ODRFactory.cpp)

target_include_directories(scabolic-core PRIVATE
        ${DGTAL_INCLUDE_DIRS}
        include
        ../../ext/include)

target_link_libraries(scabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES})

install(TARGETS scabolic-core
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX})