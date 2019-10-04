cmake_minimum_required(VERSION 3.2)
project(efinder)

add_executable(efinder efinder.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(efinder dipacus scabolic gcurve tools plot)
endif ()

target_include_directories(efinder PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/tools/include
        ${CMAKE_SOURCE_DIR}/Applications/maxEnergy-study/plot/include
        )
target_link_libraries(efinder
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

