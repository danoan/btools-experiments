cmake_minimum_required(VERSION 3.2)
project(kfinder-plotGSEqualEpsilon)

add_executable(kfinder-plotGSEqualEpsilon plotGSEqualEpsilon.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(kfinder-plotGSEqualEpsilon dipacus scabolic gcurve tools plot)
endif ()

target_include_directories(kfinder-plotGSEqualEpsilon PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/plot/include
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/tools/include
        )
target_link_libraries(kfinder-plotGSEqualEpsilon
        -ltools
        -lplot
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})