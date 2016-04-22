#ifdef __CINT__

#pragma link off all globals;
#pragma link off all class;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

//#pragma extra_include "Fit/DataRange.h"
#pragma link C++ namespace ROOT::Fit;

#pragma link C++ class ROOT::Fit::DataOutOfRange+;
#pragma link C++ class ROOT::Fit::MDataRange+;

#pragma link C++ class Parameter;
#pragma link C++ class Parameters;
#pragma link C++ class ParameterFileLoader;

#pragma link C++ class FCN_ChiSq;
#pragma link C++ class FCNWrapper;
#pragma link C++ class DataSetBuilder;
#pragma link C++ class RandomSurface;
#pragma link C++ class TFoamIntegrandWrapper;
#pragma link C++ class ROOT::Math::ParamFunctionBase;
#pragma link C++ class Plane;
#pragma link C++ class UnitConverter;
#pragma link C++ class GenIntegralChiSq;
#pragma link C++ class ParamFunctorAdapter;
#pragma link C++ class ReducedDimFunction;
#pragma link C++ class GenChiSq;
#pragma link C++ class MultiDimFunctionController;
#pragma link C++ class ReducedDimFunctionController;
#pragma link C++ class RegionFileLoader;
#pragma link C++ typedef Regions;
#pragma link C++ class RegionIntegratorMultiDim;

#pragma link C++ class SquaredFunction;
#pragma link C++ class MakePositiveSmoothFcn;
#pragma link C++ class AdditiveIntegrand;
#pragma link C++ class ProductIntegrand;

#pragma link C++ class SphCoordsIntegrand;
#pragma link C++ class SphCoordsParamIntegrand;
#pragma link C++ class SphIntegrand;

#pragma link C++ class TH2Func;
#pragma link C++ class UnitFilter;
#pragma link C++ class ProgressBar;
#pragma link C++ class LambdaChiSq;

#pragma link C++ class AngDist;
#pragma link C++ class AngDistC;
#pragma link C++ class AngDistY;

#pragma link C++ class WeightedAngDist;
#pragma link C++ class WeightedAngDistC;
#pragma link C++ class WeightedAngDistY;

#pragma link C++ class Phase1ChiSq;
#pragma link C++ class Phase1ChiSqC;

#pragma link C++ class Phase1ChiSqY;
#pragma link C++ class PolPartAngDistY;
#pragma link C++ class Phase2ChiSqY;

#pragma link C++ typedef Phase2ChiSqC::SSDPair;
#pragma link C++ class Phase2ChiSqC;
#pragma link C++ class PolPartAngDistC;

#pragma link C++ global gROOT;
#pragma link C++ global gEnv;

#endif

