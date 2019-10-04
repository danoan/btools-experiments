CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(iixmeanii)

add_executable(iixmeanii IIxMeanII.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(iixmeanii dipacus scabolic)
endif ()

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(iixmeanii dipacus scabolic)
endif ()

target_include_directories(iixmeanii PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(iixmeanii
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})