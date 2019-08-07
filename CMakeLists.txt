cmake_minimum_required(VERSION 3.8)


set(BOOST_INCLUDE_DIRS /usr/local/include/boost CACHE PATH "Boost include location path." )
set(BOOST_LIBS_DIR /usr/local/lib CACHE PATH "Boost libraries location path." )

set(DIPACUS_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ext-projects/build/include CACHE PATH "DIPaCUS include location path.")
set(DIPACUS_LIBS_DIR ${CMAKE_SOURCE_DIR}/ext-projects/build/lib CACHE PATH "DIPaCUS libraries location path.")

set(GEOC_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ext-projects/build/include CACHE PATH "geoc include location path.")

set(SCABOLIC_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/ext-projects/build/include CACHE PATH "SCaBOliC include location path.")
set(SCABOLIC_LIBS_DIR ${CMAKE_SOURCE_DIR}/ext-projects/build/lib CACHE PATH "SCaBOliC libraries location path.")

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
        ${BOOST_LIBS_DIR})

add_subdirectory(Applications/dcf-lp)
add_subdirectory(Applications/figures)
add_subdirectory(Applications/IIxMeanII)
add_subdirectory(Applications/intersection-flow)
add_subdirectory(Applications/labeling-levels)
add_subdirectory(Applications/layers-intuition)
add_subdirectory(Applications/levels-contribution)
add_subdirectory(Applications/mcf)
add_subdirectory(Applications/warm-start)
add_subdirectory(Applications/linel-add-remove)
add_subdirectory(Applications/linel-correction)
add_subdirectory(Applications/odr-linels)
add_subdirectory(Applications/morphology-flow)
add_subdirectory(Applications/fill-holes)
add_subdirectory(Applications/jmiv-illustration)
add_subdirectory(Applications/rings-and-labels)
add_subdirectory(Applications/insideness)
