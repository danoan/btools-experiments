cmake_minimum_required(VERSION 3.2)
project(plot-diff)

add_library(plot-diff SHARED
        src/plot.cpp
        src/utils.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(plot-diff dipacus scabolic gcurve tools-diff)
else()
    add_dependencies(plot-diff tools-diff)
endif ()


target_include_directories(plot-diff PRIVATE
        include)


target_include_directories(plot-diff PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/diffEnergy-study/tools/include
        )

target_link_libraries(plot-diff
        -ltools-diff
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})