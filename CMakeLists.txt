
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(PhD)


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


add_subdirectory(ROOTUtils)
add_subdirectory(NuclearChart)
add_subdirectory(FragmentDists)
add_subdirectory(FragSim232Th)
add_subdirectory(FragSimAnalysis)
add_subdirectory(Fit)
add_subdirectory(SystematicFitter)
