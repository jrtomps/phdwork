// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictFit

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "AdditiveIntegrand.h"
#include "AngDistC.h"
#include "AngDist.h"
#include "AngDistY.h"
#include "BoundaryGenerator.h"
#include "DataSetBuilder.h"
#include "FCN_ChiSq.h"
#include "FcnWrapper.h"
#include "GenChiSq.h"
#include "GenIntegralChiSq.h"
#include "LambdaChiSq.h"
#include "MakePositiveSmoothFcn.h"
#include "MDataRange.h"
#include "MultiBoundaryGenerator.h"
#include "MultiDimFunctionController.h"
#include "MultiDimVisualizer.h"
#include "ParameterFileLoader.h"
#include "Parameter.h"
#include "Parameters.h"
#include "ParamFunctorAdapter.h"
#include "Phase1ChiSqC.h"
#include "Phase1ChiSq.h"
#include "Phase1ChiSqY.h"
#include "Phase2ChiSqC.h"
#include "Phase2ChiSqY.h"
#include "Plane.h"
#include "PolPartAngDistC.h"
#include "PolPartAngDistY.h"
#include "ProductIntegrand.h"
#include "ProgressBar.h"
#include "RandomSurface.h"
#include "ReducedDimFunctionController.h"
#include "ReducedDimFunction.h"
#include "RegionIntegralChiSq.h"
#include "RegionIntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"
#include "SphIntegrand.h"
#include "SquaredFunction.h"
#include "TAngFoamIntegrandWrapper.h"
#include "TFoamIntegrandWrapper.h"
#include "TH2Func.h"
#include "UniRandomSampler.h"
#include "UnitConverter.h"
#include "UnitFilter.h"
#include "UnpolAngDistC.h"
#include "UnpolAngDist.h"
#include "WeightedAngDistC.h"
#include "WeightedAngDist.h"
#include "WeightedAngDistY.h"

// Header files passed via #pragma extra_include

namespace ROOT {
namespace Fit {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *ROOTcLcLFit_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("ROOT::Fit", 0 /*version*/, "Fit/DataRange.h", 20,
                     ::ROOT::DefineBehavior((void*)0,(void*)0),
                     &ROOTcLcLFit_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *ROOTcLcLFit_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}
}

namespace ROOT {
   static TClass *ROOTcLcLFitcLcLDataOutOfRange_Dictionary();
   static void ROOTcLcLFitcLcLDataOutOfRange_TClassManip(TClass*);
   static void *new_ROOTcLcLFitcLcLDataOutOfRange(void *p = 0);
   static void *newArray_ROOTcLcLFitcLcLDataOutOfRange(Long_t size, void *p);
   static void delete_ROOTcLcLFitcLcLDataOutOfRange(void *p);
   static void deleteArray_ROOTcLcLFitcLcLDataOutOfRange(void *p);
   static void destruct_ROOTcLcLFitcLcLDataOutOfRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ROOT::Fit::DataOutOfRange*)
   {
      ::ROOT::Fit::DataOutOfRange *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ROOT::Fit::DataOutOfRange),0);
      static ::ROOT::TGenericClassInfo 
         instance("ROOT::Fit::DataOutOfRange", "MDataRange.h", 11,
                  typeid(::ROOT::Fit::DataOutOfRange), DefineBehavior(ptr, ptr),
                  &ROOTcLcLFitcLcLDataOutOfRange_Dictionary, isa_proxy, 4,
                  sizeof(::ROOT::Fit::DataOutOfRange) );
      instance.SetNew(&new_ROOTcLcLFitcLcLDataOutOfRange);
      instance.SetNewArray(&newArray_ROOTcLcLFitcLcLDataOutOfRange);
      instance.SetDelete(&delete_ROOTcLcLFitcLcLDataOutOfRange);
      instance.SetDeleteArray(&deleteArray_ROOTcLcLFitcLcLDataOutOfRange);
      instance.SetDestructor(&destruct_ROOTcLcLFitcLcLDataOutOfRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ROOT::Fit::DataOutOfRange*)
   {
      return GenerateInitInstanceLocal((::ROOT::Fit::DataOutOfRange*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ROOT::Fit::DataOutOfRange*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOTcLcLFitcLcLDataOutOfRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ROOT::Fit::DataOutOfRange*)0x0)->GetClass();
      ROOTcLcLFitcLcLDataOutOfRange_TClassManip(theClass);
   return theClass;
   }

   static void ROOTcLcLFitcLcLDataOutOfRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ROOTcLcLFitcLcLMDataRange_Dictionary();
   static void ROOTcLcLFitcLcLMDataRange_TClassManip(TClass*);
   static void *new_ROOTcLcLFitcLcLMDataRange(void *p = 0);
   static void *newArray_ROOTcLcLFitcLcLMDataRange(Long_t size, void *p);
   static void delete_ROOTcLcLFitcLcLMDataRange(void *p);
   static void deleteArray_ROOTcLcLFitcLcLMDataRange(void *p);
   static void destruct_ROOTcLcLFitcLcLMDataRange(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ROOT::Fit::MDataRange*)
   {
      ::ROOT::Fit::MDataRange *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ROOT::Fit::MDataRange),0);
      static ::ROOT::TGenericClassInfo 
         instance("ROOT::Fit::MDataRange", "MDataRange.h", 13,
                  typeid(::ROOT::Fit::MDataRange), DefineBehavior(ptr, ptr),
                  &ROOTcLcLFitcLcLMDataRange_Dictionary, isa_proxy, 4,
                  sizeof(::ROOT::Fit::MDataRange) );
      instance.SetNew(&new_ROOTcLcLFitcLcLMDataRange);
      instance.SetNewArray(&newArray_ROOTcLcLFitcLcLMDataRange);
      instance.SetDelete(&delete_ROOTcLcLFitcLcLMDataRange);
      instance.SetDeleteArray(&deleteArray_ROOTcLcLFitcLcLMDataRange);
      instance.SetDestructor(&destruct_ROOTcLcLFitcLcLMDataRange);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ROOT::Fit::MDataRange*)
   {
      return GenerateInitInstanceLocal((::ROOT::Fit::MDataRange*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ROOT::Fit::MDataRange*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOTcLcLFitcLcLMDataRange_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ROOT::Fit::MDataRange*)0x0)->GetClass();
      ROOTcLcLFitcLcLMDataRange_TClassManip(theClass);
   return theClass;
   }

   static void ROOTcLcLFitcLcLMDataRange_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Parameter_Dictionary();
   static void Parameter_TClassManip(TClass*);
   static void *new_Parameter(void *p = 0);
   static void *newArray_Parameter(Long_t size, void *p);
   static void delete_Parameter(void *p);
   static void deleteArray_Parameter(void *p);
   static void destruct_Parameter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Parameter*)
   {
      ::Parameter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Parameter),0);
      static ::ROOT::TGenericClassInfo 
         instance("Parameter", "Parameter.h", 14,
                  typeid(::Parameter), DefineBehavior(ptr, ptr),
                  &Parameter_Dictionary, isa_proxy, 0,
                  sizeof(::Parameter) );
      instance.SetNew(&new_Parameter);
      instance.SetNewArray(&newArray_Parameter);
      instance.SetDelete(&delete_Parameter);
      instance.SetDeleteArray(&deleteArray_Parameter);
      instance.SetDestructor(&destruct_Parameter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Parameter*)
   {
      return GenerateInitInstanceLocal((::Parameter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Parameter*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Parameter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Parameter*)0x0)->GetClass();
      Parameter_TClassManip(theClass);
   return theClass;
   }

   static void Parameter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Parameters_Dictionary();
   static void Parameters_TClassManip(TClass*);
   static void *new_Parameters(void *p = 0);
   static void *newArray_Parameters(Long_t size, void *p);
   static void delete_Parameters(void *p);
   static void deleteArray_Parameters(void *p);
   static void destruct_Parameters(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Parameters*)
   {
      ::Parameters *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Parameters),0);
      static ::ROOT::TGenericClassInfo 
         instance("Parameters", "Parameters.h", 26,
                  typeid(::Parameters), DefineBehavior(ptr, ptr),
                  &Parameters_Dictionary, isa_proxy, 0,
                  sizeof(::Parameters) );
      instance.SetNew(&new_Parameters);
      instance.SetNewArray(&newArray_Parameters);
      instance.SetDelete(&delete_Parameters);
      instance.SetDeleteArray(&deleteArray_Parameters);
      instance.SetDestructor(&destruct_Parameters);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Parameters*)
   {
      return GenerateInitInstanceLocal((::Parameters*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Parameters*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Parameters_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Parameters*)0x0)->GetClass();
      Parameters_TClassManip(theClass);
   return theClass;
   }

   static void Parameters_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ParameterFileLoader_Dictionary();
   static void ParameterFileLoader_TClassManip(TClass*);
   static void *new_ParameterFileLoader(void *p = 0);
   static void *newArray_ParameterFileLoader(Long_t size, void *p);
   static void delete_ParameterFileLoader(void *p);
   static void deleteArray_ParameterFileLoader(void *p);
   static void destruct_ParameterFileLoader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ParameterFileLoader*)
   {
      ::ParameterFileLoader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ParameterFileLoader),0);
      static ::ROOT::TGenericClassInfo 
         instance("ParameterFileLoader", "ParameterFileLoader.h", 9,
                  typeid(::ParameterFileLoader), DefineBehavior(ptr, ptr),
                  &ParameterFileLoader_Dictionary, isa_proxy, 0,
                  sizeof(::ParameterFileLoader) );
      instance.SetNew(&new_ParameterFileLoader);
      instance.SetNewArray(&newArray_ParameterFileLoader);
      instance.SetDelete(&delete_ParameterFileLoader);
      instance.SetDeleteArray(&deleteArray_ParameterFileLoader);
      instance.SetDestructor(&destruct_ParameterFileLoader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ParameterFileLoader*)
   {
      return GenerateInitInstanceLocal((::ParameterFileLoader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ParameterFileLoader*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ParameterFileLoader_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ParameterFileLoader*)0x0)->GetClass();
      ParameterFileLoader_TClassManip(theClass);
   return theClass;
   }

   static void ParameterFileLoader_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FCN_ChiSq_Dictionary();
   static void FCN_ChiSq_TClassManip(TClass*);
   static void delete_FCN_ChiSq(void *p);
   static void deleteArray_FCN_ChiSq(void *p);
   static void destruct_FCN_ChiSq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FCN_ChiSq*)
   {
      ::FCN_ChiSq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FCN_ChiSq),0);
      static ::ROOT::TGenericClassInfo 
         instance("FCN_ChiSq", "FCN_ChiSq.h", 22,
                  typeid(::FCN_ChiSq), DefineBehavior(ptr, ptr),
                  &FCN_ChiSq_Dictionary, isa_proxy, 0,
                  sizeof(::FCN_ChiSq) );
      instance.SetDelete(&delete_FCN_ChiSq);
      instance.SetDeleteArray(&deleteArray_FCN_ChiSq);
      instance.SetDestructor(&destruct_FCN_ChiSq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FCN_ChiSq*)
   {
      return GenerateInitInstanceLocal((::FCN_ChiSq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::FCN_ChiSq*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FCN_ChiSq_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FCN_ChiSq*)0x0)->GetClass();
      FCN_ChiSq_TClassManip(theClass);
   return theClass;
   }

   static void FCN_ChiSq_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FCNWrapper_Dictionary();
   static void FCNWrapper_TClassManip(TClass*);
   static void delete_FCNWrapper(void *p);
   static void deleteArray_FCNWrapper(void *p);
   static void destruct_FCNWrapper(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FCNWrapper*)
   {
      ::FCNWrapper *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FCNWrapper),0);
      static ::ROOT::TGenericClassInfo 
         instance("FCNWrapper", "FcnWrapper.h", 10,
                  typeid(::FCNWrapper), DefineBehavior(ptr, ptr),
                  &FCNWrapper_Dictionary, isa_proxy, 0,
                  sizeof(::FCNWrapper) );
      instance.SetDelete(&delete_FCNWrapper);
      instance.SetDeleteArray(&deleteArray_FCNWrapper);
      instance.SetDestructor(&destruct_FCNWrapper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FCNWrapper*)
   {
      return GenerateInitInstanceLocal((::FCNWrapper*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::FCNWrapper*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FCNWrapper_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FCNWrapper*)0x0)->GetClass();
      FCNWrapper_TClassManip(theClass);
   return theClass;
   }

   static void FCNWrapper_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *DataSetBuilder_Dictionary();
   static void DataSetBuilder_TClassManip(TClass*);
   static void *new_DataSetBuilder(void *p = 0);
   static void *newArray_DataSetBuilder(Long_t size, void *p);
   static void delete_DataSetBuilder(void *p);
   static void deleteArray_DataSetBuilder(void *p);
   static void destruct_DataSetBuilder(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DataSetBuilder*)
   {
      ::DataSetBuilder *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DataSetBuilder),0);
      static ::ROOT::TGenericClassInfo 
         instance("DataSetBuilder", "DataSetBuilder.h", 41,
                  typeid(::DataSetBuilder), DefineBehavior(ptr, ptr),
                  &DataSetBuilder_Dictionary, isa_proxy, 0,
                  sizeof(::DataSetBuilder) );
      instance.SetNew(&new_DataSetBuilder);
      instance.SetNewArray(&newArray_DataSetBuilder);
      instance.SetDelete(&delete_DataSetBuilder);
      instance.SetDeleteArray(&deleteArray_DataSetBuilder);
      instance.SetDestructor(&destruct_DataSetBuilder);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DataSetBuilder*)
   {
      return GenerateInitInstanceLocal((::DataSetBuilder*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DataSetBuilder*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DataSetBuilder_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DataSetBuilder*)0x0)->GetClass();
      DataSetBuilder_TClassManip(theClass);
   return theClass;
   }

   static void DataSetBuilder_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *RandomSurface_Dictionary();
   static void RandomSurface_TClassManip(TClass*);
   static void delete_RandomSurface(void *p);
   static void deleteArray_RandomSurface(void *p);
   static void destruct_RandomSurface(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RandomSurface*)
   {
      ::RandomSurface *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::RandomSurface),0);
      static ::ROOT::TGenericClassInfo 
         instance("RandomSurface", "RandomSurface.h", 29,
                  typeid(::RandomSurface), DefineBehavior(ptr, ptr),
                  &RandomSurface_Dictionary, isa_proxy, 0,
                  sizeof(::RandomSurface) );
      instance.SetDelete(&delete_RandomSurface);
      instance.SetDeleteArray(&deleteArray_RandomSurface);
      instance.SetDestructor(&destruct_RandomSurface);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RandomSurface*)
   {
      return GenerateInitInstanceLocal((::RandomSurface*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RandomSurface*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *RandomSurface_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::RandomSurface*)0x0)->GetClass();
      RandomSurface_TClassManip(theClass);
   return theClass;
   }

   static void RandomSurface_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_TFoamIntegrandWrapper(void *p);
   static void deleteArray_TFoamIntegrandWrapper(void *p);
   static void destruct_TFoamIntegrandWrapper(void *p);
   static void streamer_TFoamIntegrandWrapper(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TFoamIntegrandWrapper*)
   {
      ::TFoamIntegrandWrapper *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TFoamIntegrandWrapper >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TFoamIntegrandWrapper", ::TFoamIntegrandWrapper::Class_Version(), "TFoamIntegrandWrapper.h", 25,
                  typeid(::TFoamIntegrandWrapper), DefineBehavior(ptr, ptr),
                  &::TFoamIntegrandWrapper::Dictionary, isa_proxy, 16,
                  sizeof(::TFoamIntegrandWrapper) );
      instance.SetDelete(&delete_TFoamIntegrandWrapper);
      instance.SetDeleteArray(&deleteArray_TFoamIntegrandWrapper);
      instance.SetDestructor(&destruct_TFoamIntegrandWrapper);
      instance.SetStreamerFunc(&streamer_TFoamIntegrandWrapper);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TFoamIntegrandWrapper*)
   {
      return GenerateInitInstanceLocal((::TFoamIntegrandWrapper*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TFoamIntegrandWrapper*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *ROOTcLcLMathcLcLParamFunctionBase_Dictionary();
   static void ROOTcLcLMathcLcLParamFunctionBase_TClassManip(TClass*);
   static void delete_ROOTcLcLMathcLcLParamFunctionBase(void *p);
   static void deleteArray_ROOTcLcLMathcLcLParamFunctionBase(void *p);
   static void destruct_ROOTcLcLMathcLcLParamFunctionBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ROOT::Math::ParamFunctionBase*)
   {
      ::ROOT::Math::ParamFunctionBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ROOT::Math::ParamFunctionBase),0);
      static ::ROOT::TGenericClassInfo 
         instance("ROOT::Math::ParamFunctionBase", "Math/ParamFunctor.h", 41,
                  typeid(::ROOT::Math::ParamFunctionBase), DefineBehavior(ptr, ptr),
                  &ROOTcLcLMathcLcLParamFunctionBase_Dictionary, isa_proxy, 0,
                  sizeof(::ROOT::Math::ParamFunctionBase) );
      instance.SetDelete(&delete_ROOTcLcLMathcLcLParamFunctionBase);
      instance.SetDeleteArray(&deleteArray_ROOTcLcLMathcLcLParamFunctionBase);
      instance.SetDestructor(&destruct_ROOTcLcLMathcLcLParamFunctionBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ROOT::Math::ParamFunctionBase*)
   {
      return GenerateInitInstanceLocal((::ROOT::Math::ParamFunctionBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ROOT::Math::ParamFunctionBase*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOTcLcLMathcLcLParamFunctionBase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ROOT::Math::ParamFunctionBase*)0x0)->GetClass();
      ROOTcLcLMathcLcLParamFunctionBase_TClassManip(theClass);
   return theClass;
   }

   static void ROOTcLcLMathcLcLParamFunctionBase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Plane_Dictionary();
   static void Plane_TClassManip(TClass*);
   static void *new_Plane(void *p = 0);
   static void *newArray_Plane(Long_t size, void *p);
   static void delete_Plane(void *p);
   static void deleteArray_Plane(void *p);
   static void destruct_Plane(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Plane*)
   {
      ::Plane *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Plane),0);
      static ::ROOT::TGenericClassInfo 
         instance("Plane", "Plane.h", 23,
                  typeid(::Plane), DefineBehavior(ptr, ptr),
                  &Plane_Dictionary, isa_proxy, 0,
                  sizeof(::Plane) );
      instance.SetNew(&new_Plane);
      instance.SetNewArray(&newArray_Plane);
      instance.SetDelete(&delete_Plane);
      instance.SetDeleteArray(&deleteArray_Plane);
      instance.SetDestructor(&destruct_Plane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Plane*)
   {
      return GenerateInitInstanceLocal((::Plane*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Plane*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Plane_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Plane*)0x0)->GetClass();
      Plane_TClassManip(theClass);
   return theClass;
   }

   static void Plane_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *UnitConverter_Dictionary();
   static void UnitConverter_TClassManip(TClass*);
   static void delete_UnitConverter(void *p);
   static void deleteArray_UnitConverter(void *p);
   static void destruct_UnitConverter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::UnitConverter*)
   {
      ::UnitConverter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::UnitConverter),0);
      static ::ROOT::TGenericClassInfo 
         instance("UnitConverter", "UnitConverter.h", 11,
                  typeid(::UnitConverter), DefineBehavior(ptr, ptr),
                  &UnitConverter_Dictionary, isa_proxy, 0,
                  sizeof(::UnitConverter) );
      instance.SetDelete(&delete_UnitConverter);
      instance.SetDeleteArray(&deleteArray_UnitConverter);
      instance.SetDestructor(&destruct_UnitConverter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::UnitConverter*)
   {
      return GenerateInitInstanceLocal((::UnitConverter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::UnitConverter*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *UnitConverter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::UnitConverter*)0x0)->GetClass();
      UnitConverter_TClassManip(theClass);
   return theClass;
   }

   static void UnitConverter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GenIntegralChiSq_Dictionary();
   static void GenIntegralChiSq_TClassManip(TClass*);
   static void delete_GenIntegralChiSq(void *p);
   static void deleteArray_GenIntegralChiSq(void *p);
   static void destruct_GenIntegralChiSq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenIntegralChiSq*)
   {
      ::GenIntegralChiSq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::GenIntegralChiSq),0);
      static ::ROOT::TGenericClassInfo 
         instance("GenIntegralChiSq", "GenIntegralChiSq.h", 34,
                  typeid(::GenIntegralChiSq), DefineBehavior(ptr, ptr),
                  &GenIntegralChiSq_Dictionary, isa_proxy, 0,
                  sizeof(::GenIntegralChiSq) );
      instance.SetDelete(&delete_GenIntegralChiSq);
      instance.SetDeleteArray(&deleteArray_GenIntegralChiSq);
      instance.SetDestructor(&destruct_GenIntegralChiSq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenIntegralChiSq*)
   {
      return GenerateInitInstanceLocal((::GenIntegralChiSq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenIntegralChiSq*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GenIntegralChiSq_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::GenIntegralChiSq*)0x0)->GetClass();
      GenIntegralChiSq_TClassManip(theClass);
   return theClass;
   }

   static void GenIntegralChiSq_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ParamFunctorAdapter_Dictionary();
   static void ParamFunctorAdapter_TClassManip(TClass*);
   static void delete_ParamFunctorAdapter(void *p);
   static void deleteArray_ParamFunctorAdapter(void *p);
   static void destruct_ParamFunctorAdapter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ParamFunctorAdapter*)
   {
      ::ParamFunctorAdapter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ParamFunctorAdapter),0);
      static ::ROOT::TGenericClassInfo 
         instance("ParamFunctorAdapter", "ParamFunctorAdapter.h", 17,
                  typeid(::ParamFunctorAdapter), DefineBehavior(ptr, ptr),
                  &ParamFunctorAdapter_Dictionary, isa_proxy, 0,
                  sizeof(::ParamFunctorAdapter) );
      instance.SetDelete(&delete_ParamFunctorAdapter);
      instance.SetDeleteArray(&deleteArray_ParamFunctorAdapter);
      instance.SetDestructor(&destruct_ParamFunctorAdapter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ParamFunctorAdapter*)
   {
      return GenerateInitInstanceLocal((::ParamFunctorAdapter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ParamFunctorAdapter*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ParamFunctorAdapter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ParamFunctorAdapter*)0x0)->GetClass();
      ParamFunctorAdapter_TClassManip(theClass);
   return theClass;
   }

   static void ParamFunctorAdapter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ReducedDimFunction_Dictionary();
   static void ReducedDimFunction_TClassManip(TClass*);
   static void delete_ReducedDimFunction(void *p);
   static void deleteArray_ReducedDimFunction(void *p);
   static void destruct_ReducedDimFunction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReducedDimFunction*)
   {
      ::ReducedDimFunction *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ReducedDimFunction),0);
      static ::ROOT::TGenericClassInfo 
         instance("ReducedDimFunction", "ReducedDimFunction.h", 34,
                  typeid(::ReducedDimFunction), DefineBehavior(ptr, ptr),
                  &ReducedDimFunction_Dictionary, isa_proxy, 0,
                  sizeof(::ReducedDimFunction) );
      instance.SetDelete(&delete_ReducedDimFunction);
      instance.SetDeleteArray(&deleteArray_ReducedDimFunction);
      instance.SetDestructor(&destruct_ReducedDimFunction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReducedDimFunction*)
   {
      return GenerateInitInstanceLocal((::ReducedDimFunction*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ReducedDimFunction*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ReducedDimFunction_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ReducedDimFunction*)0x0)->GetClass();
      ReducedDimFunction_TClassManip(theClass);
   return theClass;
   }

   static void ReducedDimFunction_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GenChiSq_Dictionary();
   static void GenChiSq_TClassManip(TClass*);
   static void delete_GenChiSq(void *p);
   static void deleteArray_GenChiSq(void *p);
   static void destruct_GenChiSq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenChiSq*)
   {
      ::GenChiSq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::GenChiSq),0);
      static ::ROOT::TGenericClassInfo 
         instance("GenChiSq", "GenChiSq.h", 19,
                  typeid(::GenChiSq), DefineBehavior(ptr, ptr),
                  &GenChiSq_Dictionary, isa_proxy, 0,
                  sizeof(::GenChiSq) );
      instance.SetDelete(&delete_GenChiSq);
      instance.SetDeleteArray(&deleteArray_GenChiSq);
      instance.SetDestructor(&destruct_GenChiSq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenChiSq*)
   {
      return GenerateInitInstanceLocal((::GenChiSq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenChiSq*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GenChiSq_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::GenChiSq*)0x0)->GetClass();
      GenChiSq_TClassManip(theClass);
   return theClass;
   }

   static void GenChiSq_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MultiDimFunctionController_Dictionary();
   static void MultiDimFunctionController_TClassManip(TClass*);
   static void delete_MultiDimFunctionController(void *p);
   static void deleteArray_MultiDimFunctionController(void *p);
   static void destruct_MultiDimFunctionController(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MultiDimFunctionController*)
   {
      ::MultiDimFunctionController *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MultiDimFunctionController),0);
      static ::ROOT::TGenericClassInfo 
         instance("MultiDimFunctionController", "MultiDimFunctionController.h", 24,
                  typeid(::MultiDimFunctionController), DefineBehavior(ptr, ptr),
                  &MultiDimFunctionController_Dictionary, isa_proxy, 0,
                  sizeof(::MultiDimFunctionController) );
      instance.SetDelete(&delete_MultiDimFunctionController);
      instance.SetDeleteArray(&deleteArray_MultiDimFunctionController);
      instance.SetDestructor(&destruct_MultiDimFunctionController);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MultiDimFunctionController*)
   {
      return GenerateInitInstanceLocal((::MultiDimFunctionController*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MultiDimFunctionController*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MultiDimFunctionController_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MultiDimFunctionController*)0x0)->GetClass();
      MultiDimFunctionController_TClassManip(theClass);
   return theClass;
   }

   static void MultiDimFunctionController_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ReducedDimFunctionController_Dictionary();
   static void ReducedDimFunctionController_TClassManip(TClass*);
   static void delete_ReducedDimFunctionController(void *p);
   static void deleteArray_ReducedDimFunctionController(void *p);
   static void destruct_ReducedDimFunctionController(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReducedDimFunctionController*)
   {
      ::ReducedDimFunctionController *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ReducedDimFunctionController),0);
      static ::ROOT::TGenericClassInfo 
         instance("ReducedDimFunctionController", "ReducedDimFunctionController.h", 25,
                  typeid(::ReducedDimFunctionController), DefineBehavior(ptr, ptr),
                  &ReducedDimFunctionController_Dictionary, isa_proxy, 0,
                  sizeof(::ReducedDimFunctionController) );
      instance.SetDelete(&delete_ReducedDimFunctionController);
      instance.SetDeleteArray(&deleteArray_ReducedDimFunctionController);
      instance.SetDestructor(&destruct_ReducedDimFunctionController);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReducedDimFunctionController*)
   {
      return GenerateInitInstanceLocal((::ReducedDimFunctionController*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ReducedDimFunctionController*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ReducedDimFunctionController_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ReducedDimFunctionController*)0x0)->GetClass();
      ReducedDimFunctionController_TClassManip(theClass);
   return theClass;
   }

   static void ReducedDimFunctionController_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *RegionFileLoader_Dictionary();
   static void RegionFileLoader_TClassManip(TClass*);
   static void *new_RegionFileLoader(void *p = 0);
   static void *newArray_RegionFileLoader(Long_t size, void *p);
   static void delete_RegionFileLoader(void *p);
   static void deleteArray_RegionFileLoader(void *p);
   static void destruct_RegionFileLoader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RegionFileLoader*)
   {
      ::RegionFileLoader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::RegionFileLoader),0);
      static ::ROOT::TGenericClassInfo 
         instance("RegionFileLoader", "RegionIntegratorMultiDim.h", 178,
                  typeid(::RegionFileLoader), DefineBehavior(ptr, ptr),
                  &RegionFileLoader_Dictionary, isa_proxy, 0,
                  sizeof(::RegionFileLoader) );
      instance.SetNew(&new_RegionFileLoader);
      instance.SetNewArray(&newArray_RegionFileLoader);
      instance.SetDelete(&delete_RegionFileLoader);
      instance.SetDeleteArray(&deleteArray_RegionFileLoader);
      instance.SetDestructor(&destruct_RegionFileLoader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RegionFileLoader*)
   {
      return GenerateInitInstanceLocal((::RegionFileLoader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RegionFileLoader*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *RegionFileLoader_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::RegionFileLoader*)0x0)->GetClass();
      RegionFileLoader_TClassManip(theClass);
   return theClass;
   }

   static void RegionFileLoader_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *RegionIntegratorMultiDim_Dictionary();
   static void RegionIntegratorMultiDim_TClassManip(TClass*);
   static void *new_RegionIntegratorMultiDim(void *p = 0);
   static void *newArray_RegionIntegratorMultiDim(Long_t size, void *p);
   static void delete_RegionIntegratorMultiDim(void *p);
   static void deleteArray_RegionIntegratorMultiDim(void *p);
   static void destruct_RegionIntegratorMultiDim(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RegionIntegratorMultiDim*)
   {
      ::RegionIntegratorMultiDim *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::RegionIntegratorMultiDim),0);
      static ::ROOT::TGenericClassInfo 
         instance("RegionIntegratorMultiDim", "RegionIntegratorMultiDim.h", 234,
                  typeid(::RegionIntegratorMultiDim), DefineBehavior(ptr, ptr),
                  &RegionIntegratorMultiDim_Dictionary, isa_proxy, 0,
                  sizeof(::RegionIntegratorMultiDim) );
      instance.SetNew(&new_RegionIntegratorMultiDim);
      instance.SetNewArray(&newArray_RegionIntegratorMultiDim);
      instance.SetDelete(&delete_RegionIntegratorMultiDim);
      instance.SetDeleteArray(&deleteArray_RegionIntegratorMultiDim);
      instance.SetDestructor(&destruct_RegionIntegratorMultiDim);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RegionIntegratorMultiDim*)
   {
      return GenerateInitInstanceLocal((::RegionIntegratorMultiDim*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RegionIntegratorMultiDim*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *RegionIntegratorMultiDim_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::RegionIntegratorMultiDim*)0x0)->GetClass();
      RegionIntegratorMultiDim_TClassManip(theClass);
   return theClass;
   }

   static void RegionIntegratorMultiDim_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SquaredFunction_Dictionary();
   static void SquaredFunction_TClassManip(TClass*);
   static void delete_SquaredFunction(void *p);
   static void deleteArray_SquaredFunction(void *p);
   static void destruct_SquaredFunction(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SquaredFunction*)
   {
      ::SquaredFunction *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SquaredFunction),0);
      static ::ROOT::TGenericClassInfo 
         instance("SquaredFunction", "SquaredFunction.h", 15,
                  typeid(::SquaredFunction), DefineBehavior(ptr, ptr),
                  &SquaredFunction_Dictionary, isa_proxy, 0,
                  sizeof(::SquaredFunction) );
      instance.SetDelete(&delete_SquaredFunction);
      instance.SetDeleteArray(&deleteArray_SquaredFunction);
      instance.SetDestructor(&destruct_SquaredFunction);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SquaredFunction*)
   {
      return GenerateInitInstanceLocal((::SquaredFunction*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SquaredFunction*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SquaredFunction_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SquaredFunction*)0x0)->GetClass();
      SquaredFunction_TClassManip(theClass);
   return theClass;
   }

   static void SquaredFunction_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MakePositiveSmoothFcn_Dictionary();
   static void MakePositiveSmoothFcn_TClassManip(TClass*);
   static void delete_MakePositiveSmoothFcn(void *p);
   static void deleteArray_MakePositiveSmoothFcn(void *p);
   static void destruct_MakePositiveSmoothFcn(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MakePositiveSmoothFcn*)
   {
      ::MakePositiveSmoothFcn *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MakePositiveSmoothFcn),0);
      static ::ROOT::TGenericClassInfo 
         instance("MakePositiveSmoothFcn", "MakePositiveSmoothFcn.h", 15,
                  typeid(::MakePositiveSmoothFcn), DefineBehavior(ptr, ptr),
                  &MakePositiveSmoothFcn_Dictionary, isa_proxy, 0,
                  sizeof(::MakePositiveSmoothFcn) );
      instance.SetDelete(&delete_MakePositiveSmoothFcn);
      instance.SetDeleteArray(&deleteArray_MakePositiveSmoothFcn);
      instance.SetDestructor(&destruct_MakePositiveSmoothFcn);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MakePositiveSmoothFcn*)
   {
      return GenerateInitInstanceLocal((::MakePositiveSmoothFcn*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MakePositiveSmoothFcn*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MakePositiveSmoothFcn_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MakePositiveSmoothFcn*)0x0)->GetClass();
      MakePositiveSmoothFcn_TClassManip(theClass);
   return theClass;
   }

   static void MakePositiveSmoothFcn_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *AdditiveIntegrand_Dictionary();
   static void AdditiveIntegrand_TClassManip(TClass*);
   static void *new_AdditiveIntegrand(void *p = 0);
   static void *newArray_AdditiveIntegrand(Long_t size, void *p);
   static void delete_AdditiveIntegrand(void *p);
   static void deleteArray_AdditiveIntegrand(void *p);
   static void destruct_AdditiveIntegrand(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AdditiveIntegrand*)
   {
      ::AdditiveIntegrand *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AdditiveIntegrand),0);
      static ::ROOT::TGenericClassInfo 
         instance("AdditiveIntegrand", "AdditiveIntegrand.h", 24,
                  typeid(::AdditiveIntegrand), DefineBehavior(ptr, ptr),
                  &AdditiveIntegrand_Dictionary, isa_proxy, 0,
                  sizeof(::AdditiveIntegrand) );
      instance.SetNew(&new_AdditiveIntegrand);
      instance.SetNewArray(&newArray_AdditiveIntegrand);
      instance.SetDelete(&delete_AdditiveIntegrand);
      instance.SetDeleteArray(&deleteArray_AdditiveIntegrand);
      instance.SetDestructor(&destruct_AdditiveIntegrand);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AdditiveIntegrand*)
   {
      return GenerateInitInstanceLocal((::AdditiveIntegrand*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AdditiveIntegrand*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AdditiveIntegrand_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AdditiveIntegrand*)0x0)->GetClass();
      AdditiveIntegrand_TClassManip(theClass);
   return theClass;
   }

   static void AdditiveIntegrand_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ProductIntegrand_Dictionary();
   static void ProductIntegrand_TClassManip(TClass*);
   static void *new_ProductIntegrand(void *p = 0);
   static void *newArray_ProductIntegrand(Long_t size, void *p);
   static void delete_ProductIntegrand(void *p);
   static void deleteArray_ProductIntegrand(void *p);
   static void destruct_ProductIntegrand(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ProductIntegrand*)
   {
      ::ProductIntegrand *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ProductIntegrand),0);
      static ::ROOT::TGenericClassInfo 
         instance("ProductIntegrand", "ProductIntegrand.h", 23,
                  typeid(::ProductIntegrand), DefineBehavior(ptr, ptr),
                  &ProductIntegrand_Dictionary, isa_proxy, 0,
                  sizeof(::ProductIntegrand) );
      instance.SetNew(&new_ProductIntegrand);
      instance.SetNewArray(&newArray_ProductIntegrand);
      instance.SetDelete(&delete_ProductIntegrand);
      instance.SetDeleteArray(&deleteArray_ProductIntegrand);
      instance.SetDestructor(&destruct_ProductIntegrand);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ProductIntegrand*)
   {
      return GenerateInitInstanceLocal((::ProductIntegrand*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ProductIntegrand*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ProductIntegrand_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ProductIntegrand*)0x0)->GetClass();
      ProductIntegrand_TClassManip(theClass);
   return theClass;
   }

   static void ProductIntegrand_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SphCoordsIntegrand_Dictionary();
   static void SphCoordsIntegrand_TClassManip(TClass*);
   static void delete_SphCoordsIntegrand(void *p);
   static void deleteArray_SphCoordsIntegrand(void *p);
   static void destruct_SphCoordsIntegrand(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SphCoordsIntegrand*)
   {
      ::SphCoordsIntegrand *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SphCoordsIntegrand),0);
      static ::ROOT::TGenericClassInfo 
         instance("SphCoordsIntegrand", "SphCoordsIntegrand.h", 17,
                  typeid(::SphCoordsIntegrand), DefineBehavior(ptr, ptr),
                  &SphCoordsIntegrand_Dictionary, isa_proxy, 0,
                  sizeof(::SphCoordsIntegrand) );
      instance.SetDelete(&delete_SphCoordsIntegrand);
      instance.SetDeleteArray(&deleteArray_SphCoordsIntegrand);
      instance.SetDestructor(&destruct_SphCoordsIntegrand);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SphCoordsIntegrand*)
   {
      return GenerateInitInstanceLocal((::SphCoordsIntegrand*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SphCoordsIntegrand*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SphCoordsIntegrand_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SphCoordsIntegrand*)0x0)->GetClass();
      SphCoordsIntegrand_TClassManip(theClass);
   return theClass;
   }

   static void SphCoordsIntegrand_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SphCoordsParamIntegrand_Dictionary();
   static void SphCoordsParamIntegrand_TClassManip(TClass*);
   static void delete_SphCoordsParamIntegrand(void *p);
   static void deleteArray_SphCoordsParamIntegrand(void *p);
   static void destruct_SphCoordsParamIntegrand(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SphCoordsParamIntegrand*)
   {
      ::SphCoordsParamIntegrand *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SphCoordsParamIntegrand),0);
      static ::ROOT::TGenericClassInfo 
         instance("SphCoordsParamIntegrand", "SphCoordsIntegrand.h", 46,
                  typeid(::SphCoordsParamIntegrand), DefineBehavior(ptr, ptr),
                  &SphCoordsParamIntegrand_Dictionary, isa_proxy, 0,
                  sizeof(::SphCoordsParamIntegrand) );
      instance.SetDelete(&delete_SphCoordsParamIntegrand);
      instance.SetDeleteArray(&deleteArray_SphCoordsParamIntegrand);
      instance.SetDestructor(&destruct_SphCoordsParamIntegrand);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SphCoordsParamIntegrand*)
   {
      return GenerateInitInstanceLocal((::SphCoordsParamIntegrand*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SphCoordsParamIntegrand*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SphCoordsParamIntegrand_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SphCoordsParamIntegrand*)0x0)->GetClass();
      SphCoordsParamIntegrand_TClassManip(theClass);
   return theClass;
   }

   static void SphCoordsParamIntegrand_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SphIntegrand_Dictionary();
   static void SphIntegrand_TClassManip(TClass*);
   static void delete_SphIntegrand(void *p);
   static void deleteArray_SphIntegrand(void *p);
   static void destruct_SphIntegrand(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SphIntegrand*)
   {
      ::SphIntegrand *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SphIntegrand),0);
      static ::ROOT::TGenericClassInfo 
         instance("SphIntegrand", "SphIntegrand.h", 15,
                  typeid(::SphIntegrand), DefineBehavior(ptr, ptr),
                  &SphIntegrand_Dictionary, isa_proxy, 0,
                  sizeof(::SphIntegrand) );
      instance.SetDelete(&delete_SphIntegrand);
      instance.SetDeleteArray(&deleteArray_SphIntegrand);
      instance.SetDestructor(&destruct_SphIntegrand);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SphIntegrand*)
   {
      return GenerateInitInstanceLocal((::SphIntegrand*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SphIntegrand*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SphIntegrand_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SphIntegrand*)0x0)->GetClass();
      SphIntegrand_TClassManip(theClass);
   return theClass;
   }

   static void SphIntegrand_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2Func_Dictionary();
   static void TH2Func_TClassManip(TClass*);
   static void delete_TH2Func(void *p);
   static void deleteArray_TH2Func(void *p);
   static void destruct_TH2Func(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2Func*)
   {
      ::TH2Func *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TH2Func),0);
      static ::ROOT::TGenericClassInfo 
         instance("TH2Func", "TH2Func.h", 17,
                  typeid(::TH2Func), DefineBehavior(ptr, ptr),
                  &TH2Func_Dictionary, isa_proxy, 0,
                  sizeof(::TH2Func) );
      instance.SetDelete(&delete_TH2Func);
      instance.SetDeleteArray(&deleteArray_TH2Func);
      instance.SetDestructor(&destruct_TH2Func);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2Func*)
   {
      return GenerateInitInstanceLocal((::TH2Func*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TH2Func*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2Func_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2Func*)0x0)->GetClass();
      TH2Func_TClassManip(theClass);
   return theClass;
   }

   static void TH2Func_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *UnitFilter_Dictionary();
   static void UnitFilter_TClassManip(TClass*);
   static void delete_UnitFilter(void *p);
   static void deleteArray_UnitFilter(void *p);
   static void destruct_UnitFilter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::UnitFilter*)
   {
      ::UnitFilter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::UnitFilter),0);
      static ::ROOT::TGenericClassInfo 
         instance("UnitFilter", "UnitFilter.h", 16,
                  typeid(::UnitFilter), DefineBehavior(ptr, ptr),
                  &UnitFilter_Dictionary, isa_proxy, 0,
                  sizeof(::UnitFilter) );
      instance.SetDelete(&delete_UnitFilter);
      instance.SetDeleteArray(&deleteArray_UnitFilter);
      instance.SetDestructor(&destruct_UnitFilter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::UnitFilter*)
   {
      return GenerateInitInstanceLocal((::UnitFilter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::UnitFilter*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *UnitFilter_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::UnitFilter*)0x0)->GetClass();
      UnitFilter_TClassManip(theClass);
   return theClass;
   }

   static void UnitFilter_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ProgressBar_Dictionary();
   static void ProgressBar_TClassManip(TClass*);
   static void delete_ProgressBar(void *p);
   static void deleteArray_ProgressBar(void *p);
   static void destruct_ProgressBar(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ProgressBar*)
   {
      ::ProgressBar *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ProgressBar),0);
      static ::ROOT::TGenericClassInfo 
         instance("ProgressBar", "ProgressBar.h", 8,
                  typeid(::ProgressBar), DefineBehavior(ptr, ptr),
                  &ProgressBar_Dictionary, isa_proxy, 0,
                  sizeof(::ProgressBar) );
      instance.SetDelete(&delete_ProgressBar);
      instance.SetDeleteArray(&deleteArray_ProgressBar);
      instance.SetDestructor(&destruct_ProgressBar);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ProgressBar*)
   {
      return GenerateInitInstanceLocal((::ProgressBar*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ProgressBar*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ProgressBar_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ProgressBar*)0x0)->GetClass();
      ProgressBar_TClassManip(theClass);
   return theClass;
   }

   static void ProgressBar_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *LambdaChiSq_Dictionary();
   static void LambdaChiSq_TClassManip(TClass*);
   static void delete_LambdaChiSq(void *p);
   static void deleteArray_LambdaChiSq(void *p);
   static void destruct_LambdaChiSq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LambdaChiSq*)
   {
      ::LambdaChiSq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::LambdaChiSq),0);
      static ::ROOT::TGenericClassInfo 
         instance("LambdaChiSq", "LambdaChiSq.h", 51,
                  typeid(::LambdaChiSq), DefineBehavior(ptr, ptr),
                  &LambdaChiSq_Dictionary, isa_proxy, 0,
                  sizeof(::LambdaChiSq) );
      instance.SetDelete(&delete_LambdaChiSq);
      instance.SetDeleteArray(&deleteArray_LambdaChiSq);
      instance.SetDestructor(&destruct_LambdaChiSq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LambdaChiSq*)
   {
      return GenerateInitInstanceLocal((::LambdaChiSq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::LambdaChiSq*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *LambdaChiSq_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::LambdaChiSq*)0x0)->GetClass();
      LambdaChiSq_TClassManip(theClass);
   return theClass;
   }

   static void LambdaChiSq_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_AngDist(void *p = 0);
   static void *newArray_AngDist(Long_t size, void *p);
   static void delete_AngDist(void *p);
   static void deleteArray_AngDist(void *p);
   static void destruct_AngDist(void *p);
   static void streamer_AngDist(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngDist*)
   {
      ::AngDist *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AngDist >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AngDist", ::AngDist::Class_Version(), "AngDist.h", 27,
                  typeid(::AngDist), DefineBehavior(ptr, ptr),
                  &::AngDist::Dictionary, isa_proxy, 16,
                  sizeof(::AngDist) );
      instance.SetNew(&new_AngDist);
      instance.SetNewArray(&newArray_AngDist);
      instance.SetDelete(&delete_AngDist);
      instance.SetDeleteArray(&deleteArray_AngDist);
      instance.SetDestructor(&destruct_AngDist);
      instance.SetStreamerFunc(&streamer_AngDist);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngDist*)
   {
      return GenerateInitInstanceLocal((::AngDist*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngDist*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AngDistC(void *p = 0);
   static void *newArray_AngDistC(Long_t size, void *p);
   static void delete_AngDistC(void *p);
   static void deleteArray_AngDistC(void *p);
   static void destruct_AngDistC(void *p);
   static void streamer_AngDistC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngDistC*)
   {
      ::AngDistC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AngDistC >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AngDistC", ::AngDistC::Class_Version(), "AngDistC.h", 27,
                  typeid(::AngDistC), DefineBehavior(ptr, ptr),
                  &::AngDistC::Dictionary, isa_proxy, 16,
                  sizeof(::AngDistC) );
      instance.SetNew(&new_AngDistC);
      instance.SetNewArray(&newArray_AngDistC);
      instance.SetDelete(&delete_AngDistC);
      instance.SetDeleteArray(&deleteArray_AngDistC);
      instance.SetDestructor(&destruct_AngDistC);
      instance.SetStreamerFunc(&streamer_AngDistC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngDistC*)
   {
      return GenerateInitInstanceLocal((::AngDistC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngDistC*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_AngDistY(void *p = 0);
   static void *newArray_AngDistY(Long_t size, void *p);
   static void delete_AngDistY(void *p);
   static void deleteArray_AngDistY(void *p);
   static void destruct_AngDistY(void *p);
   static void streamer_AngDistY(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngDistY*)
   {
      ::AngDistY *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AngDistY >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AngDistY", ::AngDistY::Class_Version(), "AngDistY.h", 27,
                  typeid(::AngDistY), DefineBehavior(ptr, ptr),
                  &::AngDistY::Dictionary, isa_proxy, 16,
                  sizeof(::AngDistY) );
      instance.SetNew(&new_AngDistY);
      instance.SetNewArray(&newArray_AngDistY);
      instance.SetDelete(&delete_AngDistY);
      instance.SetDeleteArray(&deleteArray_AngDistY);
      instance.SetDestructor(&destruct_AngDistY);
      instance.SetStreamerFunc(&streamer_AngDistY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngDistY*)
   {
      return GenerateInitInstanceLocal((::AngDistY*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngDistY*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_WeightedAngDist(void *p);
   static void deleteArray_WeightedAngDist(void *p);
   static void destruct_WeightedAngDist(void *p);
   static void streamer_WeightedAngDist(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WeightedAngDist*)
   {
      ::WeightedAngDist *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WeightedAngDist >(0);
      static ::ROOT::TGenericClassInfo 
         instance("WeightedAngDist", ::WeightedAngDist::Class_Version(), "WeightedAngDist.h", 28,
                  typeid(::WeightedAngDist), DefineBehavior(ptr, ptr),
                  &::WeightedAngDist::Dictionary, isa_proxy, 16,
                  sizeof(::WeightedAngDist) );
      instance.SetDelete(&delete_WeightedAngDist);
      instance.SetDeleteArray(&deleteArray_WeightedAngDist);
      instance.SetDestructor(&destruct_WeightedAngDist);
      instance.SetStreamerFunc(&streamer_WeightedAngDist);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WeightedAngDist*)
   {
      return GenerateInitInstanceLocal((::WeightedAngDist*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WeightedAngDist*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_WeightedAngDistC(void *p);
   static void deleteArray_WeightedAngDistC(void *p);
   static void destruct_WeightedAngDistC(void *p);
   static void streamer_WeightedAngDistC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WeightedAngDistC*)
   {
      ::WeightedAngDistC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::WeightedAngDistC >(0);
      static ::ROOT::TGenericClassInfo 
         instance("WeightedAngDistC", ::WeightedAngDistC::Class_Version(), "WeightedAngDistC.h", 28,
                  typeid(::WeightedAngDistC), DefineBehavior(ptr, ptr),
                  &::WeightedAngDistC::Dictionary, isa_proxy, 16,
                  sizeof(::WeightedAngDistC) );
      instance.SetDelete(&delete_WeightedAngDistC);
      instance.SetDeleteArray(&deleteArray_WeightedAngDistC);
      instance.SetDestructor(&destruct_WeightedAngDistC);
      instance.SetStreamerFunc(&streamer_WeightedAngDistC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WeightedAngDistC*)
   {
      return GenerateInitInstanceLocal((::WeightedAngDistC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WeightedAngDistC*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *WeightedAngDistY_Dictionary();
   static void WeightedAngDistY_TClassManip(TClass*);
   static void delete_WeightedAngDistY(void *p);
   static void deleteArray_WeightedAngDistY(void *p);
   static void destruct_WeightedAngDistY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::WeightedAngDistY*)
   {
      ::WeightedAngDistY *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::WeightedAngDistY),0);
      static ::ROOT::TGenericClassInfo 
         instance("WeightedAngDistY", "WeightedAngDistY.h", 19,
                  typeid(::WeightedAngDistY), DefineBehavior(ptr, ptr),
                  &WeightedAngDistY_Dictionary, isa_proxy, 0,
                  sizeof(::WeightedAngDistY) );
      instance.SetDelete(&delete_WeightedAngDistY);
      instance.SetDeleteArray(&deleteArray_WeightedAngDistY);
      instance.SetDestructor(&destruct_WeightedAngDistY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::WeightedAngDistY*)
   {
      return GenerateInitInstanceLocal((::WeightedAngDistY*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::WeightedAngDistY*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *WeightedAngDistY_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDistY*)0x0)->GetClass();
      WeightedAngDistY_TClassManip(theClass);
   return theClass;
   }

   static void WeightedAngDistY_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Phase1ChiSq_Dictionary();
   static void Phase1ChiSq_TClassManip(TClass*);
   static void delete_Phase1ChiSq(void *p);
   static void deleteArray_Phase1ChiSq(void *p);
   static void destruct_Phase1ChiSq(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Phase1ChiSq*)
   {
      ::Phase1ChiSq *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Phase1ChiSq),0);
      static ::ROOT::TGenericClassInfo 
         instance("Phase1ChiSq", "Phase1ChiSq.h", 51,
                  typeid(::Phase1ChiSq), DefineBehavior(ptr, ptr),
                  &Phase1ChiSq_Dictionary, isa_proxy, 0,
                  sizeof(::Phase1ChiSq) );
      instance.SetDelete(&delete_Phase1ChiSq);
      instance.SetDeleteArray(&deleteArray_Phase1ChiSq);
      instance.SetDestructor(&destruct_Phase1ChiSq);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Phase1ChiSq*)
   {
      return GenerateInitInstanceLocal((::Phase1ChiSq*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Phase1ChiSq*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Phase1ChiSq_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Phase1ChiSq*)0x0)->GetClass();
      Phase1ChiSq_TClassManip(theClass);
   return theClass;
   }

   static void Phase1ChiSq_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Phase1ChiSqC_Dictionary();
   static void Phase1ChiSqC_TClassManip(TClass*);
   static void delete_Phase1ChiSqC(void *p);
   static void deleteArray_Phase1ChiSqC(void *p);
   static void destruct_Phase1ChiSqC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Phase1ChiSqC*)
   {
      ::Phase1ChiSqC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Phase1ChiSqC),0);
      static ::ROOT::TGenericClassInfo 
         instance("Phase1ChiSqC", "Phase1ChiSqC.h", 51,
                  typeid(::Phase1ChiSqC), DefineBehavior(ptr, ptr),
                  &Phase1ChiSqC_Dictionary, isa_proxy, 0,
                  sizeof(::Phase1ChiSqC) );
      instance.SetDelete(&delete_Phase1ChiSqC);
      instance.SetDeleteArray(&deleteArray_Phase1ChiSqC);
      instance.SetDestructor(&destruct_Phase1ChiSqC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Phase1ChiSqC*)
   {
      return GenerateInitInstanceLocal((::Phase1ChiSqC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Phase1ChiSqC*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Phase1ChiSqC_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Phase1ChiSqC*)0x0)->GetClass();
      Phase1ChiSqC_TClassManip(theClass);
   return theClass;
   }

   static void Phase1ChiSqC_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Phase1ChiSqY_Dictionary();
   static void Phase1ChiSqY_TClassManip(TClass*);
   static void delete_Phase1ChiSqY(void *p);
   static void deleteArray_Phase1ChiSqY(void *p);
   static void destruct_Phase1ChiSqY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Phase1ChiSqY*)
   {
      ::Phase1ChiSqY *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Phase1ChiSqY),0);
      static ::ROOT::TGenericClassInfo 
         instance("Phase1ChiSqY", "Phase1ChiSqY.h", 51,
                  typeid(::Phase1ChiSqY), DefineBehavior(ptr, ptr),
                  &Phase1ChiSqY_Dictionary, isa_proxy, 0,
                  sizeof(::Phase1ChiSqY) );
      instance.SetDelete(&delete_Phase1ChiSqY);
      instance.SetDeleteArray(&deleteArray_Phase1ChiSqY);
      instance.SetDestructor(&destruct_Phase1ChiSqY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Phase1ChiSqY*)
   {
      return GenerateInitInstanceLocal((::Phase1ChiSqY*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Phase1ChiSqY*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Phase1ChiSqY_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Phase1ChiSqY*)0x0)->GetClass();
      Phase1ChiSqY_TClassManip(theClass);
   return theClass;
   }

   static void Phase1ChiSqY_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_PolPartAngDistY(void *p = 0);
   static void *newArray_PolPartAngDistY(Long_t size, void *p);
   static void delete_PolPartAngDistY(void *p);
   static void deleteArray_PolPartAngDistY(void *p);
   static void destruct_PolPartAngDistY(void *p);
   static void streamer_PolPartAngDistY(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PolPartAngDistY*)
   {
      ::PolPartAngDistY *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PolPartAngDistY >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PolPartAngDistY", ::PolPartAngDistY::Class_Version(), "PolPartAngDistY.h", 27,
                  typeid(::PolPartAngDistY), DefineBehavior(ptr, ptr),
                  &::PolPartAngDistY::Dictionary, isa_proxy, 16,
                  sizeof(::PolPartAngDistY) );
      instance.SetNew(&new_PolPartAngDistY);
      instance.SetNewArray(&newArray_PolPartAngDistY);
      instance.SetDelete(&delete_PolPartAngDistY);
      instance.SetDeleteArray(&deleteArray_PolPartAngDistY);
      instance.SetDestructor(&destruct_PolPartAngDistY);
      instance.SetStreamerFunc(&streamer_PolPartAngDistY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PolPartAngDistY*)
   {
      return GenerateInitInstanceLocal((::PolPartAngDistY*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PolPartAngDistY*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *Phase2ChiSqY_Dictionary();
   static void Phase2ChiSqY_TClassManip(TClass*);
   static void delete_Phase2ChiSqY(void *p);
   static void deleteArray_Phase2ChiSqY(void *p);
   static void destruct_Phase2ChiSqY(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Phase2ChiSqY*)
   {
      ::Phase2ChiSqY *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Phase2ChiSqY),0);
      static ::ROOT::TGenericClassInfo 
         instance("Phase2ChiSqY", "Phase2ChiSqY.h", 51,
                  typeid(::Phase2ChiSqY), DefineBehavior(ptr, ptr),
                  &Phase2ChiSqY_Dictionary, isa_proxy, 0,
                  sizeof(::Phase2ChiSqY) );
      instance.SetDelete(&delete_Phase2ChiSqY);
      instance.SetDeleteArray(&deleteArray_Phase2ChiSqY);
      instance.SetDestructor(&destruct_Phase2ChiSqY);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Phase2ChiSqY*)
   {
      return GenerateInitInstanceLocal((::Phase2ChiSqY*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Phase2ChiSqY*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Phase2ChiSqY_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Phase2ChiSqY*)0x0)->GetClass();
      Phase2ChiSqY_TClassManip(theClass);
   return theClass;
   }

   static void Phase2ChiSqY_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Phase2ChiSqC_Dictionary();
   static void Phase2ChiSqC_TClassManip(TClass*);
   static void delete_Phase2ChiSqC(void *p);
   static void deleteArray_Phase2ChiSqC(void *p);
   static void destruct_Phase2ChiSqC(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Phase2ChiSqC*)
   {
      ::Phase2ChiSqC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Phase2ChiSqC),0);
      static ::ROOT::TGenericClassInfo 
         instance("Phase2ChiSqC", "Phase2ChiSqC.h", 51,
                  typeid(::Phase2ChiSqC), DefineBehavior(ptr, ptr),
                  &Phase2ChiSqC_Dictionary, isa_proxy, 0,
                  sizeof(::Phase2ChiSqC) );
      instance.SetDelete(&delete_Phase2ChiSqC);
      instance.SetDeleteArray(&deleteArray_Phase2ChiSqC);
      instance.SetDestructor(&destruct_Phase2ChiSqC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Phase2ChiSqC*)
   {
      return GenerateInitInstanceLocal((::Phase2ChiSqC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Phase2ChiSqC*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Phase2ChiSqC_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Phase2ChiSqC*)0x0)->GetClass();
      Phase2ChiSqC_TClassManip(theClass);
   return theClass;
   }

   static void Phase2ChiSqC_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_PolPartAngDistC(void *p = 0);
   static void *newArray_PolPartAngDistC(Long_t size, void *p);
   static void delete_PolPartAngDistC(void *p);
   static void deleteArray_PolPartAngDistC(void *p);
   static void destruct_PolPartAngDistC(void *p);
   static void streamer_PolPartAngDistC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PolPartAngDistC*)
   {
      ::PolPartAngDistC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PolPartAngDistC >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PolPartAngDistC", ::PolPartAngDistC::Class_Version(), "PolPartAngDistC.h", 27,
                  typeid(::PolPartAngDistC), DefineBehavior(ptr, ptr),
                  &::PolPartAngDistC::Dictionary, isa_proxy, 16,
                  sizeof(::PolPartAngDistC) );
      instance.SetNew(&new_PolPartAngDistC);
      instance.SetNewArray(&newArray_PolPartAngDistC);
      instance.SetDelete(&delete_PolPartAngDistC);
      instance.SetDeleteArray(&deleteArray_PolPartAngDistC);
      instance.SetDestructor(&destruct_PolPartAngDistC);
      instance.SetStreamerFunc(&streamer_PolPartAngDistC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PolPartAngDistC*)
   {
      return GenerateInitInstanceLocal((::PolPartAngDistC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PolPartAngDistC*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TFoamIntegrandWrapper::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TFoamIntegrandWrapper::Class_Name()
{
   return "TFoamIntegrandWrapper";
}

//______________________________________________________________________________
const char *TFoamIntegrandWrapper::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TFoamIntegrandWrapper*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TFoamIntegrandWrapper::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TFoamIntegrandWrapper*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TFoamIntegrandWrapper::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TFoamIntegrandWrapper*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TFoamIntegrandWrapper::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TFoamIntegrandWrapper*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AngDist::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AngDist::Class_Name()
{
   return "AngDist";
}

//______________________________________________________________________________
const char *AngDist::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDist*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AngDist::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDist*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AngDist::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDist*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AngDist::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDist*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AngDistC::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AngDistC::Class_Name()
{
   return "AngDistC";
}

//______________________________________________________________________________
const char *AngDistC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDistC*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AngDistC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDistC*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AngDistC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDistC*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AngDistC::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDistC*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AngDistY::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AngDistY::Class_Name()
{
   return "AngDistY";
}

//______________________________________________________________________________
const char *AngDistY::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDistY*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AngDistY::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngDistY*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AngDistY::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDistY*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AngDistY::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngDistY*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr WeightedAngDist::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *WeightedAngDist::Class_Name()
{
   return "WeightedAngDist";
}

//______________________________________________________________________________
const char *WeightedAngDist::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDist*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int WeightedAngDist::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDist*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WeightedAngDist::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDist*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WeightedAngDist::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDist*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr WeightedAngDistC::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *WeightedAngDistC::Class_Name()
{
   return "WeightedAngDistC";
}

//______________________________________________________________________________
const char *WeightedAngDistC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDistC*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int WeightedAngDistC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDistC*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WeightedAngDistC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDistC*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WeightedAngDistC::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::WeightedAngDistC*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PolPartAngDistY::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PolPartAngDistY::Class_Name()
{
   return "PolPartAngDistY";
}

//______________________________________________________________________________
const char *PolPartAngDistY::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistY*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PolPartAngDistY::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistY*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PolPartAngDistY::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistY*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PolPartAngDistY::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistY*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PolPartAngDistC::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PolPartAngDistC::Class_Name()
{
   return "PolPartAngDistC";
}

//______________________________________________________________________________
const char *PolPartAngDistC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistC*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PolPartAngDistC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistC*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PolPartAngDistC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistC*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PolPartAngDistC::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PolPartAngDistC*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ROOTcLcLFitcLcLDataOutOfRange(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::ROOT::Fit::DataOutOfRange : new ::ROOT::Fit::DataOutOfRange;
   }
   static void *newArray_ROOTcLcLFitcLcLDataOutOfRange(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::ROOT::Fit::DataOutOfRange[nElements] : new ::ROOT::Fit::DataOutOfRange[nElements];
   }
   // Wrapper around operator delete
   static void delete_ROOTcLcLFitcLcLDataOutOfRange(void *p) {
      delete ((::ROOT::Fit::DataOutOfRange*)p);
   }
   static void deleteArray_ROOTcLcLFitcLcLDataOutOfRange(void *p) {
      delete [] ((::ROOT::Fit::DataOutOfRange*)p);
   }
   static void destruct_ROOTcLcLFitcLcLDataOutOfRange(void *p) {
      typedef ::ROOT::Fit::DataOutOfRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ROOT::Fit::DataOutOfRange

namespace ROOT {
   // Wrappers around operator new
   static void *new_ROOTcLcLFitcLcLMDataRange(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::ROOT::Fit::MDataRange : new ::ROOT::Fit::MDataRange;
   }
   static void *newArray_ROOTcLcLFitcLcLMDataRange(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::ROOT::Fit::MDataRange[nElements] : new ::ROOT::Fit::MDataRange[nElements];
   }
   // Wrapper around operator delete
   static void delete_ROOTcLcLFitcLcLMDataRange(void *p) {
      delete ((::ROOT::Fit::MDataRange*)p);
   }
   static void deleteArray_ROOTcLcLFitcLcLMDataRange(void *p) {
      delete [] ((::ROOT::Fit::MDataRange*)p);
   }
   static void destruct_ROOTcLcLFitcLcLMDataRange(void *p) {
      typedef ::ROOT::Fit::MDataRange current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ROOT::Fit::MDataRange

namespace ROOT {
   // Wrappers around operator new
   static void *new_Parameter(void *p) {
      return  p ? new(p) ::Parameter : new ::Parameter;
   }
   static void *newArray_Parameter(Long_t nElements, void *p) {
      return p ? new(p) ::Parameter[nElements] : new ::Parameter[nElements];
   }
   // Wrapper around operator delete
   static void delete_Parameter(void *p) {
      delete ((::Parameter*)p);
   }
   static void deleteArray_Parameter(void *p) {
      delete [] ((::Parameter*)p);
   }
   static void destruct_Parameter(void *p) {
      typedef ::Parameter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Parameter

namespace ROOT {
   // Wrappers around operator new
   static void *new_Parameters(void *p) {
      return  p ? new(p) ::Parameters : new ::Parameters;
   }
   static void *newArray_Parameters(Long_t nElements, void *p) {
      return p ? new(p) ::Parameters[nElements] : new ::Parameters[nElements];
   }
   // Wrapper around operator delete
   static void delete_Parameters(void *p) {
      delete ((::Parameters*)p);
   }
   static void deleteArray_Parameters(void *p) {
      delete [] ((::Parameters*)p);
   }
   static void destruct_Parameters(void *p) {
      typedef ::Parameters current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Parameters

namespace ROOT {
   // Wrappers around operator new
   static void *new_ParameterFileLoader(void *p) {
      return  p ? new(p) ::ParameterFileLoader : new ::ParameterFileLoader;
   }
   static void *newArray_ParameterFileLoader(Long_t nElements, void *p) {
      return p ? new(p) ::ParameterFileLoader[nElements] : new ::ParameterFileLoader[nElements];
   }
   // Wrapper around operator delete
   static void delete_ParameterFileLoader(void *p) {
      delete ((::ParameterFileLoader*)p);
   }
   static void deleteArray_ParameterFileLoader(void *p) {
      delete [] ((::ParameterFileLoader*)p);
   }
   static void destruct_ParameterFileLoader(void *p) {
      typedef ::ParameterFileLoader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ParameterFileLoader

namespace ROOT {
   // Wrapper around operator delete
   static void delete_FCN_ChiSq(void *p) {
      delete ((::FCN_ChiSq*)p);
   }
   static void deleteArray_FCN_ChiSq(void *p) {
      delete [] ((::FCN_ChiSq*)p);
   }
   static void destruct_FCN_ChiSq(void *p) {
      typedef ::FCN_ChiSq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FCN_ChiSq

namespace ROOT {
   // Wrapper around operator delete
   static void delete_FCNWrapper(void *p) {
      delete ((::FCNWrapper*)p);
   }
   static void deleteArray_FCNWrapper(void *p) {
      delete [] ((::FCNWrapper*)p);
   }
   static void destruct_FCNWrapper(void *p) {
      typedef ::FCNWrapper current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FCNWrapper

namespace ROOT {
   // Wrappers around operator new
   static void *new_DataSetBuilder(void *p) {
      return  p ? new(p) ::DataSetBuilder : new ::DataSetBuilder;
   }
   static void *newArray_DataSetBuilder(Long_t nElements, void *p) {
      return p ? new(p) ::DataSetBuilder[nElements] : new ::DataSetBuilder[nElements];
   }
   // Wrapper around operator delete
   static void delete_DataSetBuilder(void *p) {
      delete ((::DataSetBuilder*)p);
   }
   static void deleteArray_DataSetBuilder(void *p) {
      delete [] ((::DataSetBuilder*)p);
   }
   static void destruct_DataSetBuilder(void *p) {
      typedef ::DataSetBuilder current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DataSetBuilder

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RandomSurface(void *p) {
      delete ((::RandomSurface*)p);
   }
   static void deleteArray_RandomSurface(void *p) {
      delete [] ((::RandomSurface*)p);
   }
   static void destruct_RandomSurface(void *p) {
      typedef ::RandomSurface current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RandomSurface

//______________________________________________________________________________
void TFoamIntegrandWrapper::Streamer(TBuffer &R__b)
{
   // Stream an object of class TFoamIntegrandWrapper.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TFoamIntegrandWrapper(void *p) {
      delete ((::TFoamIntegrandWrapper*)p);
   }
   static void deleteArray_TFoamIntegrandWrapper(void *p) {
      delete [] ((::TFoamIntegrandWrapper*)p);
   }
   static void destruct_TFoamIntegrandWrapper(void *p) {
      typedef ::TFoamIntegrandWrapper current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TFoamIntegrandWrapper(TBuffer &buf, void *obj) {
      ((::TFoamIntegrandWrapper*)obj)->::TFoamIntegrandWrapper::Streamer(buf);
   }
} // end of namespace ROOT for class ::TFoamIntegrandWrapper

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ROOTcLcLMathcLcLParamFunctionBase(void *p) {
      delete ((::ROOT::Math::ParamFunctionBase*)p);
   }
   static void deleteArray_ROOTcLcLMathcLcLParamFunctionBase(void *p) {
      delete [] ((::ROOT::Math::ParamFunctionBase*)p);
   }
   static void destruct_ROOTcLcLMathcLcLParamFunctionBase(void *p) {
      typedef ::ROOT::Math::ParamFunctionBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ROOT::Math::ParamFunctionBase

namespace ROOT {
   // Wrappers around operator new
   static void *new_Plane(void *p) {
      return  p ? new(p) ::Plane : new ::Plane;
   }
   static void *newArray_Plane(Long_t nElements, void *p) {
      return p ? new(p) ::Plane[nElements] : new ::Plane[nElements];
   }
   // Wrapper around operator delete
   static void delete_Plane(void *p) {
      delete ((::Plane*)p);
   }
   static void deleteArray_Plane(void *p) {
      delete [] ((::Plane*)p);
   }
   static void destruct_Plane(void *p) {
      typedef ::Plane current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Plane

namespace ROOT {
   // Wrapper around operator delete
   static void delete_UnitConverter(void *p) {
      delete ((::UnitConverter*)p);
   }
   static void deleteArray_UnitConverter(void *p) {
      delete [] ((::UnitConverter*)p);
   }
   static void destruct_UnitConverter(void *p) {
      typedef ::UnitConverter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::UnitConverter

namespace ROOT {
   // Wrapper around operator delete
   static void delete_GenIntegralChiSq(void *p) {
      delete ((::GenIntegralChiSq*)p);
   }
   static void deleteArray_GenIntegralChiSq(void *p) {
      delete [] ((::GenIntegralChiSq*)p);
   }
   static void destruct_GenIntegralChiSq(void *p) {
      typedef ::GenIntegralChiSq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GenIntegralChiSq

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ParamFunctorAdapter(void *p) {
      delete ((::ParamFunctorAdapter*)p);
   }
   static void deleteArray_ParamFunctorAdapter(void *p) {
      delete [] ((::ParamFunctorAdapter*)p);
   }
   static void destruct_ParamFunctorAdapter(void *p) {
      typedef ::ParamFunctorAdapter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ParamFunctorAdapter

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ReducedDimFunction(void *p) {
      delete ((::ReducedDimFunction*)p);
   }
   static void deleteArray_ReducedDimFunction(void *p) {
      delete [] ((::ReducedDimFunction*)p);
   }
   static void destruct_ReducedDimFunction(void *p) {
      typedef ::ReducedDimFunction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ReducedDimFunction

namespace ROOT {
   // Wrapper around operator delete
   static void delete_GenChiSq(void *p) {
      delete ((::GenChiSq*)p);
   }
   static void deleteArray_GenChiSq(void *p) {
      delete [] ((::GenChiSq*)p);
   }
   static void destruct_GenChiSq(void *p) {
      typedef ::GenChiSq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GenChiSq

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MultiDimFunctionController(void *p) {
      delete ((::MultiDimFunctionController*)p);
   }
   static void deleteArray_MultiDimFunctionController(void *p) {
      delete [] ((::MultiDimFunctionController*)p);
   }
   static void destruct_MultiDimFunctionController(void *p) {
      typedef ::MultiDimFunctionController current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MultiDimFunctionController

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ReducedDimFunctionController(void *p) {
      delete ((::ReducedDimFunctionController*)p);
   }
   static void deleteArray_ReducedDimFunctionController(void *p) {
      delete [] ((::ReducedDimFunctionController*)p);
   }
   static void destruct_ReducedDimFunctionController(void *p) {
      typedef ::ReducedDimFunctionController current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ReducedDimFunctionController

namespace ROOT {
   // Wrappers around operator new
   static void *new_RegionFileLoader(void *p) {
      return  p ? new(p) ::RegionFileLoader : new ::RegionFileLoader;
   }
   static void *newArray_RegionFileLoader(Long_t nElements, void *p) {
      return p ? new(p) ::RegionFileLoader[nElements] : new ::RegionFileLoader[nElements];
   }
   // Wrapper around operator delete
   static void delete_RegionFileLoader(void *p) {
      delete ((::RegionFileLoader*)p);
   }
   static void deleteArray_RegionFileLoader(void *p) {
      delete [] ((::RegionFileLoader*)p);
   }
   static void destruct_RegionFileLoader(void *p) {
      typedef ::RegionFileLoader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RegionFileLoader

namespace ROOT {
   // Wrappers around operator new
   static void *new_RegionIntegratorMultiDim(void *p) {
      return  p ? new(p) ::RegionIntegratorMultiDim : new ::RegionIntegratorMultiDim;
   }
   static void *newArray_RegionIntegratorMultiDim(Long_t nElements, void *p) {
      return p ? new(p) ::RegionIntegratorMultiDim[nElements] : new ::RegionIntegratorMultiDim[nElements];
   }
   // Wrapper around operator delete
   static void delete_RegionIntegratorMultiDim(void *p) {
      delete ((::RegionIntegratorMultiDim*)p);
   }
   static void deleteArray_RegionIntegratorMultiDim(void *p) {
      delete [] ((::RegionIntegratorMultiDim*)p);
   }
   static void destruct_RegionIntegratorMultiDim(void *p) {
      typedef ::RegionIntegratorMultiDim current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RegionIntegratorMultiDim

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SquaredFunction(void *p) {
      delete ((::SquaredFunction*)p);
   }
   static void deleteArray_SquaredFunction(void *p) {
      delete [] ((::SquaredFunction*)p);
   }
   static void destruct_SquaredFunction(void *p) {
      typedef ::SquaredFunction current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SquaredFunction

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MakePositiveSmoothFcn(void *p) {
      delete ((::MakePositiveSmoothFcn*)p);
   }
   static void deleteArray_MakePositiveSmoothFcn(void *p) {
      delete [] ((::MakePositiveSmoothFcn*)p);
   }
   static void destruct_MakePositiveSmoothFcn(void *p) {
      typedef ::MakePositiveSmoothFcn current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MakePositiveSmoothFcn

namespace ROOT {
   // Wrappers around operator new
   static void *new_AdditiveIntegrand(void *p) {
      return  p ? new(p) ::AdditiveIntegrand : new ::AdditiveIntegrand;
   }
   static void *newArray_AdditiveIntegrand(Long_t nElements, void *p) {
      return p ? new(p) ::AdditiveIntegrand[nElements] : new ::AdditiveIntegrand[nElements];
   }
   // Wrapper around operator delete
   static void delete_AdditiveIntegrand(void *p) {
      delete ((::AdditiveIntegrand*)p);
   }
   static void deleteArray_AdditiveIntegrand(void *p) {
      delete [] ((::AdditiveIntegrand*)p);
   }
   static void destruct_AdditiveIntegrand(void *p) {
      typedef ::AdditiveIntegrand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AdditiveIntegrand

namespace ROOT {
   // Wrappers around operator new
   static void *new_ProductIntegrand(void *p) {
      return  p ? new(p) ::ProductIntegrand : new ::ProductIntegrand;
   }
   static void *newArray_ProductIntegrand(Long_t nElements, void *p) {
      return p ? new(p) ::ProductIntegrand[nElements] : new ::ProductIntegrand[nElements];
   }
   // Wrapper around operator delete
   static void delete_ProductIntegrand(void *p) {
      delete ((::ProductIntegrand*)p);
   }
   static void deleteArray_ProductIntegrand(void *p) {
      delete [] ((::ProductIntegrand*)p);
   }
   static void destruct_ProductIntegrand(void *p) {
      typedef ::ProductIntegrand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ProductIntegrand

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SphCoordsIntegrand(void *p) {
      delete ((::SphCoordsIntegrand*)p);
   }
   static void deleteArray_SphCoordsIntegrand(void *p) {
      delete [] ((::SphCoordsIntegrand*)p);
   }
   static void destruct_SphCoordsIntegrand(void *p) {
      typedef ::SphCoordsIntegrand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SphCoordsIntegrand

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SphCoordsParamIntegrand(void *p) {
      delete ((::SphCoordsParamIntegrand*)p);
   }
   static void deleteArray_SphCoordsParamIntegrand(void *p) {
      delete [] ((::SphCoordsParamIntegrand*)p);
   }
   static void destruct_SphCoordsParamIntegrand(void *p) {
      typedef ::SphCoordsParamIntegrand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SphCoordsParamIntegrand

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SphIntegrand(void *p) {
      delete ((::SphIntegrand*)p);
   }
   static void deleteArray_SphIntegrand(void *p) {
      delete [] ((::SphIntegrand*)p);
   }
   static void destruct_SphIntegrand(void *p) {
      typedef ::SphIntegrand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SphIntegrand

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TH2Func(void *p) {
      delete ((::TH2Func*)p);
   }
   static void deleteArray_TH2Func(void *p) {
      delete [] ((::TH2Func*)p);
   }
   static void destruct_TH2Func(void *p) {
      typedef ::TH2Func current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TH2Func

namespace ROOT {
   // Wrapper around operator delete
   static void delete_UnitFilter(void *p) {
      delete ((::UnitFilter*)p);
   }
   static void deleteArray_UnitFilter(void *p) {
      delete [] ((::UnitFilter*)p);
   }
   static void destruct_UnitFilter(void *p) {
      typedef ::UnitFilter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::UnitFilter

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ProgressBar(void *p) {
      delete ((::ProgressBar*)p);
   }
   static void deleteArray_ProgressBar(void *p) {
      delete [] ((::ProgressBar*)p);
   }
   static void destruct_ProgressBar(void *p) {
      typedef ::ProgressBar current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ProgressBar

namespace ROOT {
   // Wrapper around operator delete
   static void delete_LambdaChiSq(void *p) {
      delete ((::LambdaChiSq*)p);
   }
   static void deleteArray_LambdaChiSq(void *p) {
      delete [] ((::LambdaChiSq*)p);
   }
   static void destruct_LambdaChiSq(void *p) {
      typedef ::LambdaChiSq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::LambdaChiSq

//______________________________________________________________________________
void AngDist::Streamer(TBuffer &R__b)
{
   // Stream an object of class AngDist.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AngDist(void *p) {
      return  p ? new(p) ::AngDist : new ::AngDist;
   }
   static void *newArray_AngDist(Long_t nElements, void *p) {
      return p ? new(p) ::AngDist[nElements] : new ::AngDist[nElements];
   }
   // Wrapper around operator delete
   static void delete_AngDist(void *p) {
      delete ((::AngDist*)p);
   }
   static void deleteArray_AngDist(void *p) {
      delete [] ((::AngDist*)p);
   }
   static void destruct_AngDist(void *p) {
      typedef ::AngDist current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AngDist(TBuffer &buf, void *obj) {
      ((::AngDist*)obj)->::AngDist::Streamer(buf);
   }
} // end of namespace ROOT for class ::AngDist

//______________________________________________________________________________
void AngDistC::Streamer(TBuffer &R__b)
{
   // Stream an object of class AngDistC.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AngDistC(void *p) {
      return  p ? new(p) ::AngDistC : new ::AngDistC;
   }
   static void *newArray_AngDistC(Long_t nElements, void *p) {
      return p ? new(p) ::AngDistC[nElements] : new ::AngDistC[nElements];
   }
   // Wrapper around operator delete
   static void delete_AngDistC(void *p) {
      delete ((::AngDistC*)p);
   }
   static void deleteArray_AngDistC(void *p) {
      delete [] ((::AngDistC*)p);
   }
   static void destruct_AngDistC(void *p) {
      typedef ::AngDistC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AngDistC(TBuffer &buf, void *obj) {
      ((::AngDistC*)obj)->::AngDistC::Streamer(buf);
   }
} // end of namespace ROOT for class ::AngDistC

//______________________________________________________________________________
void AngDistY::Streamer(TBuffer &R__b)
{
   // Stream an object of class AngDistY.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_AngDistY(void *p) {
      return  p ? new(p) ::AngDistY : new ::AngDistY;
   }
   static void *newArray_AngDistY(Long_t nElements, void *p) {
      return p ? new(p) ::AngDistY[nElements] : new ::AngDistY[nElements];
   }
   // Wrapper around operator delete
   static void delete_AngDistY(void *p) {
      delete ((::AngDistY*)p);
   }
   static void deleteArray_AngDistY(void *p) {
      delete [] ((::AngDistY*)p);
   }
   static void destruct_AngDistY(void *p) {
      typedef ::AngDistY current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AngDistY(TBuffer &buf, void *obj) {
      ((::AngDistY*)obj)->::AngDistY::Streamer(buf);
   }
} // end of namespace ROOT for class ::AngDistY

//______________________________________________________________________________
void WeightedAngDist::Streamer(TBuffer &R__b)
{
   // Stream an object of class WeightedAngDist.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_WeightedAngDist(void *p) {
      delete ((::WeightedAngDist*)p);
   }
   static void deleteArray_WeightedAngDist(void *p) {
      delete [] ((::WeightedAngDist*)p);
   }
   static void destruct_WeightedAngDist(void *p) {
      typedef ::WeightedAngDist current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_WeightedAngDist(TBuffer &buf, void *obj) {
      ((::WeightedAngDist*)obj)->::WeightedAngDist::Streamer(buf);
   }
} // end of namespace ROOT for class ::WeightedAngDist

//______________________________________________________________________________
void WeightedAngDistC::Streamer(TBuffer &R__b)
{
   // Stream an object of class WeightedAngDistC.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_WeightedAngDistC(void *p) {
      delete ((::WeightedAngDistC*)p);
   }
   static void deleteArray_WeightedAngDistC(void *p) {
      delete [] ((::WeightedAngDistC*)p);
   }
   static void destruct_WeightedAngDistC(void *p) {
      typedef ::WeightedAngDistC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_WeightedAngDistC(TBuffer &buf, void *obj) {
      ((::WeightedAngDistC*)obj)->::WeightedAngDistC::Streamer(buf);
   }
} // end of namespace ROOT for class ::WeightedAngDistC

namespace ROOT {
   // Wrapper around operator delete
   static void delete_WeightedAngDistY(void *p) {
      delete ((::WeightedAngDistY*)p);
   }
   static void deleteArray_WeightedAngDistY(void *p) {
      delete [] ((::WeightedAngDistY*)p);
   }
   static void destruct_WeightedAngDistY(void *p) {
      typedef ::WeightedAngDistY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::WeightedAngDistY

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Phase1ChiSq(void *p) {
      delete ((::Phase1ChiSq*)p);
   }
   static void deleteArray_Phase1ChiSq(void *p) {
      delete [] ((::Phase1ChiSq*)p);
   }
   static void destruct_Phase1ChiSq(void *p) {
      typedef ::Phase1ChiSq current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Phase1ChiSq

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Phase1ChiSqC(void *p) {
      delete ((::Phase1ChiSqC*)p);
   }
   static void deleteArray_Phase1ChiSqC(void *p) {
      delete [] ((::Phase1ChiSqC*)p);
   }
   static void destruct_Phase1ChiSqC(void *p) {
      typedef ::Phase1ChiSqC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Phase1ChiSqC

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Phase1ChiSqY(void *p) {
      delete ((::Phase1ChiSqY*)p);
   }
   static void deleteArray_Phase1ChiSqY(void *p) {
      delete [] ((::Phase1ChiSqY*)p);
   }
   static void destruct_Phase1ChiSqY(void *p) {
      typedef ::Phase1ChiSqY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Phase1ChiSqY

//______________________________________________________________________________
void PolPartAngDistY::Streamer(TBuffer &R__b)
{
   // Stream an object of class PolPartAngDistY.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PolPartAngDistY(void *p) {
      return  p ? new(p) ::PolPartAngDistY : new ::PolPartAngDistY;
   }
   static void *newArray_PolPartAngDistY(Long_t nElements, void *p) {
      return p ? new(p) ::PolPartAngDistY[nElements] : new ::PolPartAngDistY[nElements];
   }
   // Wrapper around operator delete
   static void delete_PolPartAngDistY(void *p) {
      delete ((::PolPartAngDistY*)p);
   }
   static void deleteArray_PolPartAngDistY(void *p) {
      delete [] ((::PolPartAngDistY*)p);
   }
   static void destruct_PolPartAngDistY(void *p) {
      typedef ::PolPartAngDistY current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PolPartAngDistY(TBuffer &buf, void *obj) {
      ((::PolPartAngDistY*)obj)->::PolPartAngDistY::Streamer(buf);
   }
} // end of namespace ROOT for class ::PolPartAngDistY

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Phase2ChiSqY(void *p) {
      delete ((::Phase2ChiSqY*)p);
   }
   static void deleteArray_Phase2ChiSqY(void *p) {
      delete [] ((::Phase2ChiSqY*)p);
   }
   static void destruct_Phase2ChiSqY(void *p) {
      typedef ::Phase2ChiSqY current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Phase2ChiSqY

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Phase2ChiSqC(void *p) {
      delete ((::Phase2ChiSqC*)p);
   }
   static void deleteArray_Phase2ChiSqC(void *p) {
      delete [] ((::Phase2ChiSqC*)p);
   }
   static void destruct_Phase2ChiSqC(void *p) {
      typedef ::Phase2ChiSqC current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Phase2ChiSqC

//______________________________________________________________________________
void PolPartAngDistC::Streamer(TBuffer &R__b)
{
   // Stream an object of class PolPartAngDistC.

   TFoamIntegrand::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PolPartAngDistC(void *p) {
      return  p ? new(p) ::PolPartAngDistC : new ::PolPartAngDistC;
   }
   static void *newArray_PolPartAngDistC(Long_t nElements, void *p) {
      return p ? new(p) ::PolPartAngDistC[nElements] : new ::PolPartAngDistC[nElements];
   }
   // Wrapper around operator delete
   static void delete_PolPartAngDistC(void *p) {
      delete ((::PolPartAngDistC*)p);
   }
   static void deleteArray_PolPartAngDistC(void *p) {
      delete [] ((::PolPartAngDistC*)p);
   }
   static void destruct_PolPartAngDistC(void *p) {
      typedef ::PolPartAngDistC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PolPartAngDistC(TBuffer &buf, void *obj) {
      ((::PolPartAngDistC*)obj)->::PolPartAngDistC::Streamer(buf);
   }
} // end of namespace ROOT for class ::PolPartAngDistC

namespace {
  void TriggerDictionaryInitialization_dictFit_Impl() {
    static const char* headers[] = {
"AdditiveIntegrand.h",
"AngDistC.h",
"AngDist.h",
"AngDistY.h",
"BoundaryGenerator.h",
"DataSetBuilder.h",
"FCN_ChiSq.h",
"FcnWrapper.h",
"GenChiSq.h",
"GenIntegralChiSq.h",
"LambdaChiSq.h",
"MakePositiveSmoothFcn.h",
"MDataRange.h",
"MultiBoundaryGenerator.h",
"MultiDimFunctionController.h",
"MultiDimVisualizer.h",
"ParameterFileLoader.h",
"Parameter.h",
"Parameters.h",
"ParamFunctorAdapter.h",
"Phase1ChiSqC.h",
"Phase1ChiSq.h",
"Phase1ChiSqY.h",
"Phase2ChiSqC.h",
"Phase2ChiSqY.h",
"Plane.h",
"PolPartAngDistC.h",
"PolPartAngDistY.h",
"ProductIntegrand.h",
"ProgressBar.h",
"RandomSurface.h",
"ReducedDimFunctionController.h",
"ReducedDimFunction.h",
"RegionIntegralChiSq.h",
"RegionIntegratorMultiDim.h",
"SphCoordsIntegrand.h",
"SphIntegrand.h",
"SquaredFunction.h",
"TAngFoamIntegrandWrapper.h",
"TFoamIntegrandWrapper.h",
"TH2Func.h",
"UniRandomSampler.h",
"UnitConverter.h",
"UnitFilter.h",
"UnpolAngDistC.h",
"UnpolAngDist.h",
"WeightedAngDistC.h",
"WeightedAngDist.h",
"WeightedAngDistY.h",
0
    };
    static const char* includePaths[] = {
"~/PhDPaper/analysis_soft/include",
"/usr/local/CLHEP/include",
"~/PhDPaper/analysis_soft/SystematicFitter",
"/home/jeromy/Code/include",
"/usr/local/root/root-6.02.03/include",
"/home/jeromy/PhDPaper/analysis_soft/Fit/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace ROOT{namespace Fit{class __attribute__((annotate("$clingAutoload$AngDistC.h")))  DataOutOfRange;}}
namespace ROOT{namespace Fit{class __attribute__((annotate("$clingAutoload$AngDistC.h")))  MDataRange;}}
class __attribute__((annotate("$clingAutoload$AngDistC.h")))  Parameter;
class __attribute__((annotate("$clingAutoload$AngDistC.h")))  Parameters;
class __attribute__((annotate("$clingAutoload$ParameterFileLoader.h")))  ParameterFileLoader;
class __attribute__((annotate("$clingAutoload$FCN_ChiSq.h")))  FCN_ChiSq;
class __attribute__((annotate("$clingAutoload$FcnWrapper.h")))  FCNWrapper;
class __attribute__((annotate("$clingAutoload$DataSetBuilder.h")))  DataSetBuilder;
class __attribute__((annotate("$clingAutoload$RandomSurface.h")))  RandomSurface;
class __attribute__((annotate("$clingAutoload$DataSetBuilder.h")))  TFoamIntegrandWrapper;
namespace ROOT{namespace Math{class __attribute__((annotate("$clingAutoload$MultiDimVisualizer.h")))  ParamFunctionBase;}}
class __attribute__((annotate("$clingAutoload$Plane.h")))  Plane;
class __attribute__((annotate("$clingAutoload$UnitConverter.h")))  UnitConverter;
class __attribute__((annotate("$clingAutoload$GenIntegralChiSq.h")))  GenIntegralChiSq;
class __attribute__((annotate("$clingAutoload$ParamFunctorAdapter.h")))  ParamFunctorAdapter;
class __attribute__((annotate("$clingAutoload$ReducedDimFunction.h")))  ReducedDimFunction;
class __attribute__((annotate("$clingAutoload$GenChiSq.h")))  GenChiSq;
class __attribute__((annotate("$clingAutoload$MultiDimFunctionController.h")))  MultiDimFunctionController;
class __attribute__((annotate("$clingAutoload$ReducedDimFunctionController.h")))  ReducedDimFunctionController;
class __attribute__((annotate("$clingAutoload$BoundaryGenerator.h")))  RegionFileLoader;
class __attribute__((annotate("$clingAutoload$BoundaryGenerator.h")))  RegionIntegratorMultiDim;
class __attribute__((annotate("$clingAutoload$SquaredFunction.h")))  SquaredFunction;
class __attribute__((annotate("$clingAutoload$MakePositiveSmoothFcn.h")))  MakePositiveSmoothFcn;
class __attribute__((annotate("$clingAutoload$AdditiveIntegrand.h")))  AdditiveIntegrand;
class __attribute__((annotate("$clingAutoload$LambdaChiSq.h")))  ProductIntegrand;
class __attribute__((annotate("$clingAutoload$SphCoordsIntegrand.h")))  SphCoordsIntegrand;
class __attribute__((annotate("$clingAutoload$SphCoordsIntegrand.h")))  SphCoordsParamIntegrand;
class __attribute__((annotate("$clingAutoload$SphIntegrand.h")))  SphIntegrand;
class __attribute__((annotate("$clingAutoload$TH2Func.h")))  TH2Func;
class __attribute__((annotate("$clingAutoload$UnitFilter.h")))  UnitFilter;
class __attribute__((annotate("$clingAutoload$ProgressBar.h")))  ProgressBar;
class __attribute__((annotate("$clingAutoload$LambdaChiSq.h")))  LambdaChiSq;
class __attribute__((annotate("$clingAutoload$AngDist.h")))  AngDist;
class __attribute__((annotate("$clingAutoload$AngDistC.h")))  AngDistC;
class __attribute__((annotate("$clingAutoload$AngDistY.h")))  AngDistY;
class __attribute__((annotate("$clingAutoload$WeightedAngDist.h")))  WeightedAngDist;
class __attribute__((annotate("$clingAutoload$WeightedAngDistC.h")))  WeightedAngDistC;
class __attribute__((annotate("$clingAutoload$WeightedAngDistY.h")))  WeightedAngDistY;
class __attribute__((annotate("$clingAutoload$Phase1ChiSq.h")))  Phase1ChiSq;
class __attribute__((annotate("$clingAutoload$Phase1ChiSqC.h")))  Phase1ChiSqC;
class __attribute__((annotate("$clingAutoload$Phase1ChiSqY.h")))  Phase1ChiSqY;
class __attribute__((annotate("$clingAutoload$PolPartAngDistY.h")))  PolPartAngDistY;
class __attribute__((annotate("$clingAutoload$Phase2ChiSqY.h")))  Phase2ChiSqY;
class __attribute__((annotate("$clingAutoload$Phase2ChiSqC.h")))  Phase2ChiSqC;
class __attribute__((annotate("$clingAutoload$PolPartAngDistC.h")))  PolPartAngDistC;
struct __attribute__((annotate("$clingAutoload$BoundaryGenerator.h")))  Region;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "AdditiveIntegrand.h"
#include "AngDistC.h"
#include "AngDist.h"
#include "AngDistY.h"
#include "BoundaryGenerator.h"
#include "DataSetBuilder.h"
#include "FCN_ChiSq.h"
#include "FcnWrapper.h"
#include "GenChiSq.h"
#include "GenIntegralChiSq.h"
#include "LambdaChiSq.h"
#include "MakePositiveSmoothFcn.h"
#include "MDataRange.h"
#include "MultiBoundaryGenerator.h"
#include "MultiDimFunctionController.h"
#include "MultiDimVisualizer.h"
#include "ParameterFileLoader.h"
#include "Parameter.h"
#include "Parameters.h"
#include "ParamFunctorAdapter.h"
#include "Phase1ChiSqC.h"
#include "Phase1ChiSq.h"
#include "Phase1ChiSqY.h"
#include "Phase2ChiSqC.h"
#include "Phase2ChiSqY.h"
#include "Plane.h"
#include "PolPartAngDistC.h"
#include "PolPartAngDistY.h"
#include "ProductIntegrand.h"
#include "ProgressBar.h"
#include "RandomSurface.h"
#include "ReducedDimFunctionController.h"
#include "ReducedDimFunction.h"
#include "RegionIntegralChiSq.h"
#include "RegionIntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"
#include "SphIntegrand.h"
#include "SquaredFunction.h"
#include "TAngFoamIntegrandWrapper.h"
#include "TFoamIntegrandWrapper.h"
#include "TH2Func.h"
#include "UniRandomSampler.h"
#include "UnitConverter.h"
#include "UnitFilter.h"
#include "UnpolAngDistC.h"
#include "UnpolAngDist.h"
#include "WeightedAngDistC.h"
#include "WeightedAngDist.h"
#include "WeightedAngDistY.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AdditiveIntegrand", payloadCode, "@",
"AngDist", payloadCode, "@",
"AngDistC", payloadCode, "@",
"AngDistY", payloadCode, "@",
"DataSetBuilder", payloadCode, "@",
"FCNWrapper", payloadCode, "@",
"FCN_ChiSq", payloadCode, "@",
"GenChiSq", payloadCode, "@",
"GenIntegralChiSq", payloadCode, "@",
"LambdaChiSq", payloadCode, "@",
"MakePositiveSmoothFcn", payloadCode, "@",
"MultiDimFunctionController", payloadCode, "@",
"ParamFunctorAdapter", payloadCode, "@",
"Parameter", payloadCode, "@",
"ParameterFileLoader", payloadCode, "@",
"Parameters", payloadCode, "@",
"Phase1ChiSq", payloadCode, "@",
"Phase1ChiSqC", payloadCode, "@",
"Phase1ChiSqY", payloadCode, "@",
"Phase2ChiSqC", payloadCode, "@",
"Phase2ChiSqY", payloadCode, "@",
"Plane", payloadCode, "@",
"PolPartAngDistC", payloadCode, "@",
"PolPartAngDistY", payloadCode, "@",
"ProductIntegrand", payloadCode, "@",
"ProgressBar", payloadCode, "@",
"ROOT::Fit::DataOutOfRange", payloadCode, "@",
"ROOT::Fit::MDataRange", payloadCode, "@",
"ROOT::Math::ParamFunctionBase", payloadCode, "@",
"RandomSurface", payloadCode, "@",
"ReducedDimFunction", payloadCode, "@",
"ReducedDimFunctionController", payloadCode, "@",
"RegionFileLoader", payloadCode, "@",
"RegionIntegratorMultiDim", payloadCode, "@",
"Regions", payloadCode, "@",
"SphCoordsIntegrand", payloadCode, "@",
"SphCoordsParamIntegrand", payloadCode, "@",
"SphIntegrand", payloadCode, "@",
"SquaredFunction", payloadCode, "@",
"TFoamIntegrandWrapper", payloadCode, "@",
"TH2Func", payloadCode, "@",
"UnitConverter", payloadCode, "@",
"UnitFilter", payloadCode, "@",
"WeightedAngDist", payloadCode, "@",
"WeightedAngDistC", payloadCode, "@",
"WeightedAngDistY", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictFit",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictFit_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictFit_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictFit() {
  TriggerDictionaryInitialization_dictFit_Impl();
}
