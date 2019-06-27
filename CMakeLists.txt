cmake_minimum_required(VERSION 3.8)
project(Experiments)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

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
