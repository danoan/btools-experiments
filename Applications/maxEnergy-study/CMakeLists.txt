cmake_minimum_required(VERSION 3.2)
project(epsilon-finder)

add_executable(epsilon-finder efinder.cpp
        src/utils.cpp
        src/Estimations.cpp
        src/RootFinder.cpp
        src/Constants.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(epsilon-finder dipacus scabolic gcurve)
endif ()

target_include_directories(epsilon-finder PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
)
target_link_libraries(epsilon-finder
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})










add_executable(kfinder kfinder.cpp
        src/Constants.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(kfinder dipacus scabolic gcurve)
endif ()

target_include_directories(kfinder PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )
target_link_libraries(kfinder
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})