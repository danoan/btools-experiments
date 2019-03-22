cmake_minimum_required(VERSION 3.8)
project(MCF)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)


add_executable(ii-unusual ii-unusual-behavior.cpp)

target_include_directories(ii-unusual PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${GEOC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
)

target_link_libraries(ii-unusual
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})


add_executable(mcf mcf.cpp)

target_include_directories(mcf PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${GEOC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries(mcf
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

