CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
project(diff-energy)

add_executable(diff-energy diffEnergy.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(diff-energy dipacus scabolic gcurve tools-diff plot-diff)
else()
    add_dependencies(diff-energy tools-diff plot-diff)
endif ()

target_include_directories(diff-energy PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/diffEnergy-study/plot/include
        ${CMAKE_SOURCE_DIR}/Applications/diffEnergy-study/tools/include
        )
target_link_libraries(diff-energy
        -ltools-diff
        -lplot-diff
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

add_subdirectory(plot)
add_subdirectory(tools)
add_subdirectory(plotConsistent)