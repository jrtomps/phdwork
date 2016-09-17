
set(Fit_SOURCES 
  AdditiveIntegrand.cpp
  BoundaryGenerator.cpp
  DataSetBuilder.cpp
  FCN_ChiSq.cpp
  FcnWrapper.cpp
  GenChiSq.cpp
  GenIntegralChiSq.cpp
  LambdaChiSq.cpp
  MakePositiveSmoothFcn.cpp
  MultiBoundaryGenerator.cpp
  MultiDimFunctionController.cpp
  MultiDimVisualizer.cpp
  ParamFunctorAdapter.cpp
  Phase1ChiSq.cpp
  Phase1ChiSqC.cpp
  Phase1ChiSqY.cpp
  Phase2ChiSqC.cpp
  Phase2ChiSqY.cpp
  Plane.cpp
  PolPartAngDistC.cpp
  PolPartAngDistY.cpp
  ProductIntegrand.cpp
  ProgressBar.cpp
  RandomSurface.cpp
  ReducedDimFunction.cpp
  ReducedDimFunctionController.cpp
  RegionIntegralChiSq.cpp
  RegionIntegratorMultiDim.cpp
  SquaredFunction.cpp
  TFoamIntegrandWrapper.cpp
  UniRandomSampler.cpp
  UnitConverter.cpp
  UnitFilter.cpp
  UnpolAngDist.cpp
  UnpolAngDistC.cpp
  WeightedAngDist.cpp
  WeightedAngDistC.cpp
  WeightedAngDistY.cpp
  dictFit.cxx)

set(Fit_HEADERS
  AdditiveIntegrand.h
  BoundaryGenerator.h
  ConverterMultiDim.h
  DataSetBuilder.h
  FCN_ChiSq.h
  FcnWrapper.h
  GenChiSq.h
  GenIntegralChiSq.h
  LambdaChiSq.h
  MakePositiveSmoothFcn.h
  MultiBoundaryGenerator.h
  MultiDimFunctionController.h
  MultiDimVisualizer.h
  ParamFunctorAdapter.h
  Phase1ChiSq.h
  Phase1ChiSqC.h
  Phase1ChiSqY.h
  Phase2ChiSqC.h
  Phase2ChiSqY.h
  Plane.h
  PolPartAngDistC.h
  PolPartAngDistY.h
  Polarity.h
  ProductIntegrand.h
  ProgressBar.h
  RandomSurface.h
  ReducedDimFunction.h
  ReducedDimFunctionController.h
  RegionIntegralChiSq.h
  RegionIntegratorMultiDim.h
  SquaredFunction.h
  TFoamIntegrandWrapper.h
  UniRandomSampler.h
  UnitConverter.h
  UnitFilter.h
  UnpolAngDist.h
  UnpolAngDistC.h
  WeightedAngDist.h
  WeightedAngDistC.h
  WeightedAngDistY.h)

set(dictFit_INCLUDES
    -I${CLHEP_INCLUDE_DIRS}
    -I${PROJECT_SOURCE_DIR}/FragmentDists)
   
  ROOT_GENERATE_DICTIONARY(dictFit ${Fit_HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/LinkDef.h OPTIONS -c ${dictFit_INCLUDES})

include_directories(${CLHEP_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/FragmentDists ${PROJECT_SOURCE_DIR}/ROOTUtils)
add_library(Fit SHARED ${Fit_SOURCES})
target_link_libraries(Fit FragmentDists ROOTUtils)
install(TARGETS Fit LIBRARY DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictFit_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdictFit.rootmap DESTINATION lib)
