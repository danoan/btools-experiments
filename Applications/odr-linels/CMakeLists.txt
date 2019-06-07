cmake_minimum_required(VERSION 3.2)
project(odr-linels)

add_executable(odr-linels
        odr-linels.cpp
        src/FlowControl.cpp)

target_include_directories(odr-linels PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(odr-linels
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

INSTALL(TARGETS odr-linels
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)