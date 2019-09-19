cmake_minimum_required(VERSION 3.2)
project(morphology-flow)

add_executable(morphology-flow morphology-flow.cpp)

target_include_directories(morphology-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(morphology-flow
        -lbtools-core
        -lbtools-reader
        -lbtools-utils
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})