cmake_minimum_required(VERSION 3.8)
project(figures)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(figures linel-pixel.cpp)

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







add_executable(cell-grid-model cell-grid-model.cpp)

target_compile_definitions(cell-grid-model PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(cell-grid-model PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(cell-grid-model
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})




add_executable(flow-intuition flow-intuition.cpp)

target_compile_definitions(flow-intuition PRIVATE PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

target_include_directories(flow-intuition PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(flow-intuition
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})