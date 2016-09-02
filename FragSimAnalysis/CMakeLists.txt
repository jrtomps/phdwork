
set(FragSimAnalyzer_SOURCES 
    SiStripGenerator.cpp
    AngleAverager.cpp
    SolidAngleComputer.cpp
    EnergyHist.cpp 
    TTreeProxy.cpp
    Analyzer.cpp
    LambdaGenerator.cpp
    EtaDbFiller2C.cpp
    SolidAnglePDFGeneratorC.cpp
    YieldGenerator.cpp
    RatioVsOrigin.cpp
    EnergyHistSorted.cpp
    dictFragSimAnalyzer.cxx)

set(FragSimAnalyzer_HEADERS
    TSelectorInsert.h 
    SiStripGenerator.h
    AngleAverager.h
    SolidAngleComputer.h
    EnergyHist.h
    TTreeProxy.h
    Analyzer.h
    LambdaGenerator.h
    EtaDbFiller2C.h
    SolidAnglePDFGeneratorC.h
    YieldGenerator.h
    RatioVsOrigin.h
    EnergyHistSorted.h )

set(dict_INCLUDES  
    -I${PROJECT_SOURCE_DIR}/NuclearChart 
    -I${PROJECT_SOURCE_DIR}/FragSim232Th/include
    -I${PROJECT_BINARY_DIR})

  option(DBG_AngleAverager "Enable debugging output for angle averager")
  option(DBG_SolidAngleComputer "Enable debugging output for solid angle computer")
  option(DBG_Analyzer "Enable debugging output for analyzer")

  if(DBG_AngleAverager) 
    add_definitions(-DDEBUG_AngleAverager)
  endif()

  if(DBG_SolidAngleComputer) 
    add_definitions(-DDEBUG_SolidAngleComputer)
  endif()

  if(DBG_Analyzer) 
    add_definitions(-DDEBUG_Analyzer)
  endif()

include(${Geant4_USE_FILE})
ROOT_GENERATE_DICTIONARY(dictFragSimAnalyzer ${FragSimAnalyzer_HEADERS} 
                          LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h OPTIONS -c ${dict_INCLUDES})

find_package(ROOT CONFIG REQUIRED COMPONENTS ProofPlayer)
include(${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/NuclearChart 
                    ${PROJECT_SOURCE_DIR}/ROOTUtils
                    ${PROJECT_SOURCE_DIR}/FragSim232Th/include
                    ${PROJECT_BINARY_DIR})

add_library(FragSimAnalyzer SHARED ${FragSimAnalyzer_SOURCES})
target_link_libraries(FragSimAnalyzer ${ROOT_LIBRARIES} ${CLHEP_LIBRARIES} ROOTUtils) 


set(analyze_SOURCES analyze.cpp)
add_executable(analyze ${analyze_SOURCES})
target_link_libraries(analyze FragSimAnalyzer ROOTUtils NuclearChart FragSim232Th)

set(analyze_lambda_SOURCES analyze_lambda.cpp)
add_executable(analyze_lambda ${analyze_lambda_SOURCES})
target_link_libraries(analyze_lambda FragSimAnalyzer ROOTUtils NuclearChart FragSim232Th)

install(TARGETS analyze analyze_lambda DESTINATION bin)
install(TARGETS FragSimAnalyzer DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictFragSimAnalyzer_rdict.pcm DESTINATION lib)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/angle_data DESTINATION share) 
