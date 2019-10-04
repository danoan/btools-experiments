include(ExternalProject)

include(ext-projects/cmake-files/DIPaCUS.cmake)
include(ext-projects/cmake-files/SCaBOliC.cmake)
#If building maxEnergy-study, use grid-adjustement version of SCaBOliC
#include(ext-projects/cmake-files/SCaBOliC-grid-adjustement.cmake)
include(ext-projects/cmake-files/GEOC.cmake)
include(ext-projects/cmake-files/GCurve.cmake)
include(ext-projects/cmake-files/magLac.cmake)
include(ext-projects/cmake-files/exhaustive-gc.cmake)
include(ext-projects/cmake-files/BTools.cmake)