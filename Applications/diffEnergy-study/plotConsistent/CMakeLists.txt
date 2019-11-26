cmake_minimum_required(VERSION 3.2)
project(diff-plotConsistent)

add_executable(diff-plotConsistent plotConsistent.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(diff-plotConsistent dipacus scabolic gcurve tools-diff plot-diff)
else()
    add_dependencies(diff-plotConsistent tools-diff plot-diff)
endif ()

target_include_directories(diff-plotConsistent PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/diffEnergy-study/plot/include
        ${CMAKE_SOURCE_DIR}/Applications/diffEnergy-study/tools/include
        )
target_link_libraries(diff-plotConsistent
        -ltools-diff
        -lplot-diff
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        -lpthread
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})