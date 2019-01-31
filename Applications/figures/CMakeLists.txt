cmake_minimum_required(VERSION 3.8)
project(figures)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(figures figures.cpp)

target_compile_definitions(figures PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(figures PRIVATE
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