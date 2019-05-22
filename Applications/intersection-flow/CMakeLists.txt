cmake_minimum_required(VERSION 3.2)
project(intersection-flow)

add_executable(intersection-flow intersection-flow.cpp)

target_include_directories(intersection-flow PRIVATE
        include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(intersection-flow
        -lbtools-core
        -lbtools-reader
        -lbtools-utils
        -ldipacus-derivates
        -ldipacus-base
        -lscabolic-energy
        -lscabolic-core
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

install(TARGETS intersection-flow
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)