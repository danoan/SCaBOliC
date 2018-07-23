cmake_minimum_required(VERSION 2.6)
project(BCE)

set(LOCAL_LIBS_FOLDER /home/daniel/Projects/local_libs)
#set(LOCAL_LIBS_FOLDER /home-local/dantu1/Documents/local_libs)

set(QBSCC_FOLDER /home/daniel/Projects/GIT/QBSCC)
#set(QBSCC_FOLDER /home-local/dantu1/GIT/QBSCC)


set(CMAKE_PREFIX_PATH ${LOCAL_LIBS_FOLDER})
set(CMAKE_LIBRARY_PATH ${LOCAL_LIBS_FOLDER}/lib)


find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)


INCLUDE_DIRECTORIES(${LOCAL_LIBS_FOLDER}/include)
LINK_DIRECTORIES(${LOCAL_LIBS_FOLDER}/lib)


INCLUDE_DIRECTORIES(${QBSCC_FOLDER} ${QBSCC_FOLDER}/Solvers)
LINK_DIRECTORIES(${QBSCC_FOLDER}/lib/)


INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})



set(SOURCE_FILES
        bce.cpp
        bce.h
        Core/BCEInput.h
        Core/display.h
        Core/display.cpp
        Regularization/Solution.h
        Core/ODRFactory.h
        Core/ODRFactory.cpp)

add_executable(testBCE testBCE.cpp ${SOURCE_FILES})
target_link_libraries(testBCE ${DGTAL_LIBRARIES} ${OpenCV_LIBS} -lqpbo -lm -lboost_system -lboost_filesystem)




include(GNUInstallDirs)
add_library(bce STATIC bce.cpp Core/display.cpp Core/ODRFactory.cpp)

set_target_properties(bce PROPERTIES COMPILE_FLAGS "-std=c++11")


configure_file(bce.pc.in bce.pc @ONLY)

install(TARGETS bce
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES bce.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bce)
install(FILES Core/BCEInput.h Core/display.h Core/ODRFactory.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bce/Core)

install(DIRECTORY Regularization DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bce)
install(DIRECTORY ${QBSCC_FOLDER}/Solvers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bce)
install(DIRECTORY ${QBSCC_FOLDER}/ImageProc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/bce)
