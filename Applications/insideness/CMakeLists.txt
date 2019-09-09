cmake_minimum_required(VERSION 3.2)
project(insideness)

add_executable(insideness
        insideness.cpp
        src/aproxCounting.cpp
        src/counting.cpp
        src/assumeLabeling.cpp
        src/distance.cpp
        src/around.cpp)

add_dependencies(insideness dipacus scabolic btools)

target_include_directories(insideness PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${BOOST_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(insideness
        -ldipacus-base
        -ldipacus-components
        -ldipacus-derivates
        -lscabolic-energy
        -lscabolic-optimization
        -lbtools-utils
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})