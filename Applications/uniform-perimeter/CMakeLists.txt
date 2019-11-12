cmake_minimum_required(VERSION 3.2)
project(uniform-perimeter)

add_executable(uniform-perimeter uniform-perimeter.cpp
		src/FlowControl.cpp
		src/DataWriter.cpp)

if (${USE_REMOTE_REPOSITORIES})
    add_dependencies(uniform-perimeter dipacus scabolic btools gcurve)
endif ()

target_include_directories(uniform-perimeter PRIVATE
		include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
		${BTOOLS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(uniform-perimeter
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-energy
        -lscabolic-core
		-lscabolic-utils
		-lbtools-core
		-lbtools-utils
		-lbtools-reader
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

