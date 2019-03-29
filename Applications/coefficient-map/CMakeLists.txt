cmake_minimum_required(VERSION 3.8)
project(coefficient-map)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)


add_executable(coefficient-map coefficient-map.cpp)

target_compile_definitions(coefficient-map PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(coefficient-map PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(coefficient-map
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-core
        -lscabolic-energy
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})