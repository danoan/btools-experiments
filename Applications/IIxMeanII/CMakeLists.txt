CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
project(iixmeanii)

add_executable(iixmeanii IIxMeanII.cpp)

target_include_directories(iixmeanii PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/ext/include
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(iixmeanii
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})