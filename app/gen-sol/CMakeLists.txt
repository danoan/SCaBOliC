cmake_minimum_required(VERSION 3.2)
project(gen-sol)

add_executable(gen-sol gen-sol.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(gen-sol dipacus)
endif ()


target_include_directories(gen-sol PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(gen-sol
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${OpenCV_LIBS}
        ${DGTAL_LIBRARIES})