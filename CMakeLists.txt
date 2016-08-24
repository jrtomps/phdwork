
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(PhD)
set(PhD_VERSION 2.0)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else ()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(ROOT CONFIG REQUIRED)
include(${ROOT_USE_FILE})
list(APPEND CMAKE_MODULE_PATH ${ROOT_DIR}/modules)

find_package(CLHEP REQUIRED)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 CONFIG REQUIRED ui_all vis_all)
else()
  find_package(Geant4 CONFIG REQUIRED)
endif()

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

find_package(Boost REQUIRED COMPONENTS regex)


add_subdirectory(ROOTUtils)
add_subdirectory(NuclearChart)
add_subdirectory(FragmentDists)
add_subdirectory(FragSim232Th)
add_subdirectory(FragSimAnalysis)
add_subdirectory(Fit)
add_subdirectory(SystematicFitter)
add_subdirectory(Fitters)


#include(CMakePackageConfigHelpers)
#write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/PhDConfigVersion.cmake" VERSION ${PhD_VERSION} 
#COMPATIBILITY AnyNewerVersion)

#export(EXPORT PhDTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/PhDTargets.cmake")
#configure_file(cmake/PhDConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/PhDConfig.cmake" COPY_ONLY)
#set(ConfigPackageLocation lib/cmake)

#install(EXPORT PhDTargets FILE PhDTargets.cmake NAMESPACE PhD:: DESTINATION ${ConfigPackageLocation})
#install(FILES cmake/PhDConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/PhDConfigVersion.cmake"
#  DESTINATION ${ConfigPackageLocation} COMPONENT Devel)

