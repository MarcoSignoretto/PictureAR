
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Project)

if($ENV{BUILD_NUMBER})
  set(BUILD_NUMBER $ENV{BUILD_NUMBER})
else()
  set(BUILD_NUMBER 0)
endif()

set(CvLab_NAME "project")
set(CvLab_VERSION "0.1.0.${BUILD_NUMBER}")
set(CvLab_BRIEF "${CV2017_NAME} is the project of the computer vision course 2017")

set (CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/boundary.cpp
        include/feature_drawer.cpp
        include/boundary_extractor.cpp
        include/utils.cpp
        include/marker.cpp
        )

# Setup default installation dir
#
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
get_filename_component( INSTALL_DIR_ABS "${PROJECT_SOURCE_DIR}/dist" ABSOLUTE)
SET( CMAKE_INSTALL_PREFIX ${INSTALL_DIR_ABS} CACHE PATH "Install prefix" FORCE )
ENDIF()

FIND_PACKAGE( OpenCV REQUIRED )
IF( OpenCV_FOUND )
    MESSAGE( STATUS "OpenCV Version ${OpenCV_VERSION} found.")
ENDIF()

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
add_subdirectory(src)

