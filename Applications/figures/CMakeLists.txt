cmake_minimum_required(VERSION 3.8)
project(figures)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_libraries(${BOOST_LIBS_DIR})

add_executable(figures main.cpp src/cell-grid-model.cpp src/flow-intuition.cpp src/linel-pixel.cpp src/utils.cpp src/multigrid.cpp)

target_compile_definitions(figures PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(figures PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(figures
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})