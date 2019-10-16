CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(dt-flow)

add_executable(dt-flow dt-flow.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(dt-flow dipacus scabolic)
endif ()

target_include_directories(dt-flow PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(dt-flow
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})