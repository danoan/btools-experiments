cmake_minimum_required(VERSION 3.2)
project(tools-diff)

add_library(tools-diff SHARED
        src/Constants.cpp
        src/Estimations.cpp
        src/utils.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(tools-diff dipacus scabolic gcurve)
endif ()


target_include_directories(tools-diff PRIVATE
        include)


target_include_directories(tools-diff PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(tools-diff
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})