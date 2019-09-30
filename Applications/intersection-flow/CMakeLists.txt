cmake_minimum_required(VERSION 3.2)
project(intersection-flow)

add_executable(intersection-flow intersection-flow.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(intersection-flow dipacus scabolic btools)
endif ()

target_include_directories(intersection-flow PRIVATE
        include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(intersection-flow
        -lbtools-core
        -lbtools-reader
        -lbtools-utils
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-energy
        -lscabolic-core
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS intersection-flow
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)