cmake_minimum_required(VERSION 3.2)
project(generate-shapes)


add_executable(generate-shapes generate-shapes.cpp)
if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(generate-shapes dipacus input-reader)
else()
    add_dependencies(generate-shapes input-reader)
endif()

target_include_directories(generate-shapes PRIVATE
        ${CMAKE_SOURCE_DIR}/app/input-reader/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(generate-shapes
        -linput-reader
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})

