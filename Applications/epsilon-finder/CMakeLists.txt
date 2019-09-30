cmake_minimum_required(VERSION 3.2)
project(epsilon-finder)

add_executable(epsilon-finder epsilon-finder.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(epsilon-finder dipacus scabolic)
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