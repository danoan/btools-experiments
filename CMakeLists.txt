cmake_minimum_required(VERSION 3.8)

set(EXTPROJECTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext-projects/source)

if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-release)
else()
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-debug)
endif()

set(BOOST_INCLUDE_DIRS /usr/local/include/boost CACHE PATH "Boost include location path." )
set(BOOST_LIBS_DIR /usr/local/lib CACHE PATH "Boost libraries location path." )

set(DIPACUS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "geoc include location path.")

set(SCABOLIC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "SCaBOliC include location path.")
set(SCABOLIC_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "SCaBOliC libraries location path.")

set(USE_REMOTE_REPOSITORIES ON CACHE BOOL "Pull external repositories from GitHub.")

project(Experiments)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

if(${USE_REMOTE_REPOSITORIES})
    include(ext-projects/ExtProjects.cmake)
endif()

link_directories(${DIPACUS_LIBS_DIR}
        ${GEOC_LIBS_DIR}
        ${SCABOLIC_LIBS_DIR}
        ${BOOST_LIBS_DIR}
        ${CMAKE_BINARY_DIR}/Applications/maxEnergy-study/tools
        ${CMAKE_BINARY_DIR}/Applications/maxEnergy-study/plot)

add_subdirectory(Applications/IIxMeanII)
add_subdirectory(Applications/intersection-flow)
add_subdirectory(Applications/labeling-levels)
add_subdirectory(Applications/morphology-flow)
add_subdirectory(Applications/maxEnergy-study)
add_subdirectory(Applications/DT-Flow)
