cmake_minimum_required(VERSION 3.8)
project(dcf-lp)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)


add_executable(dcf-lp dcf-lp.cpp)

target_compile_definitions(dcf-lp PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(dcf-lp PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(dcf-lp
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-optimization
        -lscabolic-core
        -lscabolic-energy
        -lboost_filesystem
        -lboost_system
        -lglpk
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})