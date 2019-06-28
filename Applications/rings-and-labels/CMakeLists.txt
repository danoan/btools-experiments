cmake_minimum_required(VERSION 3.2)
project(rings-and-labels)


set(INCLUDE_DIRS
        include
        ${BOOST_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

set(LINK_LIBS -lbtools-reader
        -lbtools-core
        -lbtools-visitors
        -lbtools-utils
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lexhaustive-gc-api
        -lexhaustive-gc-energy
        -lexhaustive-gc-core
        -lgcurve
        -lgcurve-utils
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

add_executable(individual-flow individualFlow.cpp
        src/energyUtils.cpp
        src/countUnlabeled.cpp
        src/utils.cpp)

target_include_directories(individual-flow PRIVATE ${INCLUDE_DIRS})
target_link_libraries(individual-flow ${LINK_LIBS})



add_executable(model-flow modelFlow.cpp
        src/energyUtils.cpp
        src/countUnlabeled.cpp
        src/utils.cpp)

target_include_directories(model-flow PRIVATE ${INCLUDE_DIRS})
target_link_libraries(model-flow ${LINK_LIBS})