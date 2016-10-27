

set(ROOTUtils_SOURCES
  STDatime.cpp
  HistFiller.cpp
  ROOTUtils.cpp
  VectorSorter.cpp
  VectorWriter.cpp
  ColumnFileLoader.cpp
  DataBucket.cpp
  TreeMaker.cpp
  SingleTreeMaker.cpp
  MultiTreeMaker.cpp
  ConsistentAxisConverter.cpp
  TH2GaussianSmear.cpp
  TH2CosPhiSmear.cpp
  ColumnFileReader.cpp
  dictROOTUtils.cxx)

set(ROOTUtils_HEADERS
  STDatime.h
  HistFiller.h
  ROOTUtils.h
  VectorSorter.h
  VectorWriter.h
  ColumnFileLoader.h
  DataBucket.h
  TreeMaker.h
  SingleTreeMaker.h
  MultiTreeMaker.h
  ConsistentAxisConverter.h
  TH2GaussianSmear.h
  ColumnFileReader.h
  TH2CosPhiSmear.h)

ROOT_GENERATE_DICTIONARY(dictROOTUtils ${ROOTUtils_HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h OPTIONS -c)

include_directories(${ROOT_INCLUDE_DIRS})

add_library(ROOTUtils SHARED ${ROOTUtils_SOURCES})
target_link_libraries(ROOTUtils ${ROOT_LIBRARIES})

install(TARGETS ROOTUtils DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictROOTUtils_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictROOTUtils.rootmap DESTINATION lib)


