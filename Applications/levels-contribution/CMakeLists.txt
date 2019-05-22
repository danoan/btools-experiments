cmake_minimum_required(VERSION 3.2)
project(levels-contribution)

add_executable(levels-contribution levels-contribution.cpp src/FlowControl.cpp)

target_include_directories(levels-contribution PRIVATE
        include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(levels-contribution
        -lbtools-utils
        -lbtools-core
        -lbtools-reader
        -lbtools-visitors
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

INSTALL(TARGETS levels-contribution
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)