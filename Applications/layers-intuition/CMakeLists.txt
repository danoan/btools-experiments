cmake_minimum_required(VERSION 3.8)
project(layers-intuition)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(layers-intuition layers-intuition.cpp)

target_compile_definitions(layers-intuition PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(layers-intuition PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${GEOC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(layers-intuition
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})
