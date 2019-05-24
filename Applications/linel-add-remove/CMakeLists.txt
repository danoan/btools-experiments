cmake_minimum_required(VERSION 3.2)
project(linel-add-remove)

add_executable(linel-add-remove linel-add-remove.cpp)

target_include_directories(linel-add-remove PRIVATE
        include
        ${DGTAL_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS})

target_link_libraries(linel-add-remove
        -lbtools-utils
        -lscabolic-optimization
        -lscabolic-energy
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})