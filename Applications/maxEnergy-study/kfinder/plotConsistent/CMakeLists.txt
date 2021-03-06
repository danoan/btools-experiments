cmake_minimum_required(VERSION 3.2)
project(kfinder-plotConsistent)

add_executable(kfinder-plotConsistent plotConsistent.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(kfinder-plotConsistent dipacus scabolic gcurve tools plot)
else()
    add_dependencies(kfinder-plotConsistent tools plot)
endif ()

target_include_directories(kfinder-plotConsistent PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/plot/include
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/tools/include
        )
target_link_libraries(kfinder-plotConsistent
        -ltools
        -lplot
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