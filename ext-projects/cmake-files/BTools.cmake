ExternalProject_Add(btools
        GIT_REPOSITORY https://github.com/danoan/BTools.git
        GIT_TAG v0.1
        SOURCE_DIR ${EXTPROJECTS_SOURCE_DIR}/BTools
        CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${EXTPROJECTS_BUILD_DIR}
        -DUSE_REMOTE_REPOSITORIES=OFF
        -DDIPACUS_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DDIPACUS_LIBS_DIR=${EXTPROJECTS_BUILD_DIR}/lib
        -DSCABOLIC_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DSCABOLIC_LIBS_DIR=${EXTPROJECTS_BUILD_DIR}/lib
        -DBOOST_INCLUDE_DIRS=${BOOST_INCLUDE_DIRS}
        -DBOOST_LIBS_DIR=${BOOST_LIBS_DIR}
        -DGEOC_INCLUDE_DIRS=${EXTPROJECTS_BUILD_DIR}/include
        -DCREATE_SHARED_LIBRARIES=ON)

add_dependencies(btools scabolic dipacus geoc magLac exhaustive-gc)