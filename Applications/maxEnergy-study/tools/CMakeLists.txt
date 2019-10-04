cmake_minimum_required(VERSION 3.2)
project(tools)

add_library(tools SHARED
        src/RootFinder.cpp
        src/Constants.cpp
        src/Estimations.cpp
        src/utils.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(tools dipacus scabolic gcurve)
endif ()


target_include_directories(tools PRIVATE
        include)


target_include_directories(tools PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(tools
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})