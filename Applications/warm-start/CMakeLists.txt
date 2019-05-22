cmake_minimum_required(VERSION 3.2)
project(warm-start)

add_executable(warm-start warm-start.cpp)

target_include_directories(warm-start PRIVATE
        include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(warm-start
        -lbtools-core
        -lbtools-reader
        -lbtools-visitors
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