
set(Analyzer_SOURCES
  ConfigEntry.cpp ConfigManager.cpp ProofPlot.cpp PP2d.cpp 
			PPCoincMap.cpp PPCoincMapRegions.cpp SelectPPCoincMapRegions.cpp 
           PAnalysis.cpp 
           Visualizer.cpp PFVisualizer.cpp ScalerTool.cpp 
           SScalerTool.cpp Comment.cpp FNameManager.cpp ExtensibleDb.cpp 
           DataElement.cpp Utils.cpp CombinationEngine.cpp 
			MultiplicityGroupGenerator.cpp FluxCalculator.cpp 
			ADCRegionInspector.cpp  StripInfo.cpp dictAnalyzer.cxx)

set(Analyzer_HEADERS
    ConfigEntry.h ConfigManager.h ProofPlot.h PP2d.h PPCoincMap.h 
			  PPCoincMapRegions.h SelectPPCoincMapRegions.h PAnalysis.h
              Visualizer.h PFVisualizer.h 
              ScalerTool.h SScalerTool.h Comment.h FNameManager.h 
              ExtensibleDb.h DataElement.h Utils.h CombinationEngine.h 
			  MultiplicityGroupGenerator.h FluxCalculator.h 
				ADCRegionInspector.h StripInfo.h UtilTemplates.h)

ROOT_GENERATE_DICTIONARY(dictAnalyzer ${Analyzer_HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h OPTIONS -c)

include_directories(${CMAKE_SOURCE_DIR}/ROOTUtils ${ROOT_INCLUDE_DIRS})

add_library(Analyzer SHARED ${Analyzer_SOURCES})
target_link_libraries(Analyzer ${ROOT_LIBRARIES})

install(TARGETS Analyzer LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictAnalyzer_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictAnalyzer.rootmap DESTINATION lib)
install(FILES ${Analyzer_HEADERS} DESTINATION include)
