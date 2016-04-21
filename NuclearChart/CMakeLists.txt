

set(NuclearChart_SOURCES
    NuclearChart.cpp
    Isotope.cpp
    dictNuclearChart.cxx)

set(NuclearChart_HEADERS
    NuclearChart.h
    Isotope.h)

ROOT_GENERATE_DICTIONARY(dictNuclearChart ${NuclearChart_HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h OPTIONS -c)

include_directories(${ROOT_INCLUDE_DIRS})
add_library(NuclearChart SHARED ${NuclearChart_SOURCES})
