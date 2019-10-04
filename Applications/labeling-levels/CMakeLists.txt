cmake_minimum_required(VERSION 3.8)
project(labeling-levels)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(labeling-levels
        labeling-levels.cpp
        src/InputReader.cpp
        src/dispUtils.cpp
        src/energyUtils.cpp
        src/countUnlabeled.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(labeling-levels dipacus scabolic)
endif ()

target_include_directories(labeling-levels PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(labeling-levels
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-core
        -lscabolic-energy
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})