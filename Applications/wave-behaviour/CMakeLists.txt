cmake_minimum_required(VERSION 3.2)
project(wave-behaviour)

add_executable(wave-behaviour wave-behaviour.cpp)
target_include_directories(wave-behaviour PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(wave-behaviour
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})