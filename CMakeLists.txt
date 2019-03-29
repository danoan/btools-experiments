cmake_minimum_required(VERSION 3.8)
project(Experiments)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(${DIPACUS_LIBS_DIR}
        ${GEOC_LIBS_DIR}
        ${SCABOLIC_LIBS_DIR}
        ${BOOST_LIBS_DIR})

add_subdirectory(Applications/IIxMeanII)
add_subdirectory(Applications/figures)
add_subdirectory(Applications/mcf)
add_subdirectory(Applications/coefficient-map)