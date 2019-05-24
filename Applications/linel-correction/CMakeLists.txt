cmake_minimum_required(VERSION 3.2)
project(linel-correction)

add_executable(linel-correction linel-correction.cpp)

target_include_directories(linel-correction PRIVATE
        include
        ${DGTAL_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS})

target_link_libraries(linel-correction
        -lbtools-utils
        -lscabolic-optimization
        -lscabolic-energy
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})