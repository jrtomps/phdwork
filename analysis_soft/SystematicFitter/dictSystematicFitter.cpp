// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictSystematicFitter

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
#include "FitParameterGenerator.h"
#include "RandFitParamGenerator.h"
#include "SysFitLogger.h"
#include "TreeMaker.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TreeMaker_Dictionary();
   static void TreeMaker_TClassManip(TClass*);
   static void delete_TreeMaker(void *p);
   static void deleteArray_TreeMaker(void *p);
   static void destruct_TreeMaker(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TreeMaker*)
   {
      ::TreeMaker *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TreeMaker),0);
      static ::ROOT::TGenericClassInfo 
         instance("TreeMaker", "TreeMaker.h", 12,
                  typeid(::TreeMaker), DefineBehavior(ptr, ptr),
                  &TreeMaker_Dictionary, isa_proxy, 0,
                  sizeof(::TreeMaker) );
      instance.SetDelete(&delete_TreeMaker);
      instance.SetDeleteArray(&deleteArray_TreeMaker);
      instance.SetDestructor(&destruct_TreeMaker);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TreeMaker*)
   {
      return GenerateInitInstanceLocal((::TreeMaker*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TreeMaker*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TreeMaker_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TreeMaker*)0x0)->GetClass();
      TreeMaker_TClassManip(theClass);
   return theClass;
   }

   static void TreeMaker_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SysFitLogger_Dictionary();
   static void SysFitLogger_TClassManip(TClass*);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SysFitLogger*)
   {
      ::SysFitLogger *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SysFitLogger),0);
      static ::ROOT::TGenericClassInfo 
         instance("SysFitLogger", "SysFitLogger.h", 10,
                  typeid(::SysFitLogger), DefineBehavior(ptr, ptr),
                  &SysFitLogger_Dictionary, isa_proxy, 0,
                  sizeof(::SysFitLogger) );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SysFitLogger*)
   {
      return GenerateInitInstanceLocal((::SysFitLogger*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SysFitLogger*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SysFitLogger_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SysFitLogger*)0x0)->GetClass();
      SysFitLogger_TClassManip(theClass);
   return theClass;
   }

   static void SysFitLogger_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TreeMaker(void *p) {
      delete ((::TreeMaker*)p);
   }
   static void deleteArray_TreeMaker(void *p) {
      delete [] ((::TreeMaker*)p);
   }
   static void destruct_TreeMaker(void *p) {
      typedef ::TreeMaker current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TreeMaker

namespace ROOT {
} // end of namespace ROOT for class ::SysFitLogger

namespace {
  void TriggerDictionaryInitialization_dictSystematicFitter_Impl() {
    static const char* headers[] = {
"FitParameterGenerator.h",
"RandFitParamGenerator.h",
"SysFitLogger.h",
"TreeMaker.h",
0
    };
    static const char* includePaths[] = {
"/home/jeromy/PhDPaper/analysis_soft/include",
"/usr/local/root/root-6.02.03/include",
"/home/jeromy/PhDPaper/analysis_soft/SystematicFitter/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$SysFitLogger.h")))  TreeMaker;
class __attribute__((annotate("$clingAutoload$SysFitLogger.h")))  SysFitLogger;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "FitParameterGenerator.h"
#include "RandFitParamGenerator.h"
#include "SysFitLogger.h"
#include "TreeMaker.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"SysFitLogger", payloadCode, "@",
"TreeMaker", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictSystematicFitter",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictSystematicFitter_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictSystematicFitter_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictSystematicFitter() {
  TriggerDictionaryInitialization_dictSystematicFitter_Impl();
}
