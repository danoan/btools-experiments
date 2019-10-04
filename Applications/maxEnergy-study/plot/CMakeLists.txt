cmake_minimum_required(VERSION 3.2)
project(plot)

add_library(plot SHARED
        src/plot.cpp
        src/utils.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(plot dipacus scabolic gcurve tools)
endif ()


target_include_directories(plot PRIVATE
        include)


target_include_directories(plot PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/tools/include
        )

target_link_libraries(plot
        -ltools
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})