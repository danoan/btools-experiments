cmake_minimum_required(VERSION 3.2)
project(reconnection-model)

add_executable(reconnection-model
        reconnection-model.cpp
        src/InputReader.cpp)

target_include_directories(reconnection-model PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(reconnection-model
        -lbtools-core
        -lbtools-reader
        -lbtools-visitors
        -lbtools-utils
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lscabolic-utils
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})