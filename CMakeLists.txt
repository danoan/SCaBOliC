cmake_minimum_required(VERSION 3.0)
project(SCaBOliC)

#It expects variables
#CMAKE_PREFIX_PATH: find_[package] macro directories
#CMAKE_INSTALL_PREFIX: install location
#EXTRA_LINK_DIR: extra link directories
#BUILD_PATH: location of build files

link_directories(${EXTRA_LINK_DIR}
        ${BUILD_PATH}/modules/Optimization
        ${BUILD_PATH}/modules/API
        ${BUILD_PATH}/modules/Core
        ${BUILD_PATH}/modules/Energy)

add_subdirectory(modules/Core)
add_subdirectory(modules/Optimization)
add_subdirectory(modules/Energy)
add_subdirectory(modules/API)

add_subdirectory(test)

