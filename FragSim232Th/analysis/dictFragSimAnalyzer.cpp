// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictFragSimAnalyzer

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
#include "TSelectorInsert.h"
#include "SiStripGenerator.h"
#include "AngleAverager.h"
#include "SolidAngleComputer.h"
#include "EnergyHist.h"
#include "TTreeProxy.h"
#include "Analyzer.h"
#include "TH2Func.h"
#include "LambdaGenerator.h"
#include "Eta2C.h"
#include "EtaDbFiller2C.h"
#include "AngMarginalEta2C.h"
#include "SolidAnglePDFGeneratorC.h"
#include "YieldGenerator.h"
#include "RatioVsOrigin.h"
#include "EnergyHistSorted.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_TSelectorInsert(void *p);
   static void deleteArray_TSelectorInsert(void *p);
   static void destruct_TSelectorInsert(void *p);
   static void streamer_TSelectorInsert(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSelectorInsert*)
   {
      ::TSelectorInsert *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSelectorInsert >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSelectorInsert", ::TSelectorInsert::Class_Version(), "TSelectorInsert.h", 17,
                  typeid(::TSelectorInsert), DefineBehavior(ptr, ptr),
                  &::TSelectorInsert::Dictionary, isa_proxy, 16,
                  sizeof(::TSelectorInsert) );
      instance.SetDelete(&delete_TSelectorInsert);
      instance.SetDeleteArray(&deleteArray_TSelectorInsert);
      instance.SetDestructor(&destruct_TSelectorInsert);
      instance.SetStreamerFunc(&streamer_TSelectorInsert);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSelectorInsert*)
   {
      return GenerateInitInstanceLocal((::TSelectorInsert*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TSelectorInsert*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_SiStripGenerator(void *p);
   static void deleteArray_SiStripGenerator(void *p);
   static void destruct_SiStripGenerator(void *p);
   static void streamer_SiStripGenerator(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SiStripGenerator*)
   {
      ::SiStripGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SiStripGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SiStripGenerator", ::SiStripGenerator::Class_Version(), "SiStripGenerator.h", 21,
                  typeid(::SiStripGenerator), DefineBehavior(ptr, ptr),
                  &::SiStripGenerator::Dictionary, isa_proxy, 16,
                  sizeof(::SiStripGenerator) );
      instance.SetDelete(&delete_SiStripGenerator);
      instance.SetDeleteArray(&deleteArray_SiStripGenerator);
      instance.SetDestructor(&destruct_SiStripGenerator);
      instance.SetStreamerFunc(&streamer_SiStripGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SiStripGenerator*)
   {
      return GenerateInitInstanceLocal((::SiStripGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SiStripGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_AngleAverager(void *p);
   static void deleteArray_AngleAverager(void *p);
   static void destruct_AngleAverager(void *p);
   static void streamer_AngleAverager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngleAverager*)
   {
      ::AngleAverager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::AngleAverager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("AngleAverager", ::AngleAverager::Class_Version(), "AngleAverager.h", 27,
                  typeid(::AngleAverager), DefineBehavior(ptr, ptr),
                  &::AngleAverager::Dictionary, isa_proxy, 16,
                  sizeof(::AngleAverager) );
      instance.SetDelete(&delete_AngleAverager);
      instance.SetDeleteArray(&deleteArray_AngleAverager);
      instance.SetDestructor(&destruct_AngleAverager);
      instance.SetStreamerFunc(&streamer_AngleAverager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngleAverager*)
   {
      return GenerateInitInstanceLocal((::AngleAverager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngleAverager*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *SolidAngleComputer_Dictionary();
   static void SolidAngleComputer_TClassManip(TClass*);
   static void delete_SolidAngleComputer(void *p);
   static void deleteArray_SolidAngleComputer(void *p);
   static void destruct_SolidAngleComputer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SolidAngleComputer*)
   {
      ::SolidAngleComputer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SolidAngleComputer),0);
      static ::ROOT::TGenericClassInfo 
         instance("SolidAngleComputer", "SolidAngleComputer.h", 21,
                  typeid(::SolidAngleComputer), DefineBehavior(ptr, ptr),
                  &SolidAngleComputer_Dictionary, isa_proxy, 0,
                  sizeof(::SolidAngleComputer) );
      instance.SetDelete(&delete_SolidAngleComputer);
      instance.SetDeleteArray(&deleteArray_SolidAngleComputer);
      instance.SetDestructor(&destruct_SolidAngleComputer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SolidAngleComputer*)
   {
      return GenerateInitInstanceLocal((::SolidAngleComputer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SolidAngleComputer*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SolidAngleComputer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SolidAngleComputer*)0x0)->GetClass();
      SolidAngleComputer_TClassManip(theClass);
   return theClass;
   }

   static void SolidAngleComputer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_EnergyHist(void *p);
   static void deleteArray_EnergyHist(void *p);
   static void destruct_EnergyHist(void *p);
   static void streamer_EnergyHist(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnergyHist*)
   {
      ::EnergyHist *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EnergyHist >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EnergyHist", ::EnergyHist::Class_Version(), "EnergyHist.h", 22,
                  typeid(::EnergyHist), DefineBehavior(ptr, ptr),
                  &::EnergyHist::Dictionary, isa_proxy, 16,
                  sizeof(::EnergyHist) );
      instance.SetDelete(&delete_EnergyHist);
      instance.SetDeleteArray(&deleteArray_EnergyHist);
      instance.SetDestructor(&destruct_EnergyHist);
      instance.SetStreamerFunc(&streamer_EnergyHist);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnergyHist*)
   {
      return GenerateInitInstanceLocal((::EnergyHist*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EnergyHist*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *TTreeProxy_Dictionary();
   static void TTreeProxy_TClassManip(TClass*);
   static void *new_TTreeProxy(void *p = 0);
   static void *newArray_TTreeProxy(Long_t size, void *p);
   static void delete_TTreeProxy(void *p);
   static void deleteArray_TTreeProxy(void *p);
   static void destruct_TTreeProxy(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TTreeProxy*)
   {
      ::TTreeProxy *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TTreeProxy),0);
      static ::ROOT::TGenericClassInfo 
         instance("TTreeProxy", "TTreeProxy.h", 18,
                  typeid(::TTreeProxy), DefineBehavior(ptr, ptr),
                  &TTreeProxy_Dictionary, isa_proxy, 0,
                  sizeof(::TTreeProxy) );
      instance.SetNew(&new_TTreeProxy);
      instance.SetNewArray(&newArray_TTreeProxy);
      instance.SetDelete(&delete_TTreeProxy);
      instance.SetDeleteArray(&deleteArray_TTreeProxy);
      instance.SetDestructor(&destruct_TTreeProxy);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TTreeProxy*)
   {
      return GenerateInitInstanceLocal((::TTreeProxy*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TTreeProxy*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TTreeProxy_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TTreeProxy*)0x0)->GetClass();
      TTreeProxy_TClassManip(theClass);
   return theClass;
   }

   static void TTreeProxy_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_Analyzer(void *p = 0);
   static void *newArray_Analyzer(Long_t size, void *p);
   static void delete_Analyzer(void *p);
   static void deleteArray_Analyzer(void *p);
   static void destruct_Analyzer(void *p);
   static void streamer_Analyzer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analyzer*)
   {
      ::Analyzer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Analyzer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Analyzer", ::Analyzer::Class_Version(), "Analyzer.h", 20,
                  typeid(::Analyzer), DefineBehavior(ptr, ptr),
                  &::Analyzer::Dictionary, isa_proxy, 16,
                  sizeof(::Analyzer) );
      instance.SetNew(&new_Analyzer);
      instance.SetNewArray(&newArray_Analyzer);
      instance.SetDelete(&delete_Analyzer);
      instance.SetDeleteArray(&deleteArray_Analyzer);
      instance.SetDestructor(&destruct_Analyzer);
      instance.SetStreamerFunc(&streamer_Analyzer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analyzer*)
   {
      return GenerateInitInstanceLocal((::Analyzer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analyzer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *FS232ThcLcLTH2Func_Dictionary();
   static void FS232ThcLcLTH2Func_TClassManip(TClass*);
   static void delete_FS232ThcLcLTH2Func(void *p);
   static void deleteArray_FS232ThcLcLTH2Func(void *p);
   static void destruct_FS232ThcLcLTH2Func(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FS232Th::TH2Func*)
   {
      ::FS232Th::TH2Func *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FS232Th::TH2Func),0);
      static ::ROOT::TGenericClassInfo 
         instance("FS232Th::TH2Func", "TH2Func.h", 19,
                  typeid(::FS232Th::TH2Func), DefineBehavior(ptr, ptr),
                  &FS232ThcLcLTH2Func_Dictionary, isa_proxy, 0,
                  sizeof(::FS232Th::TH2Func) );
      instance.SetDelete(&delete_FS232ThcLcLTH2Func);
      instance.SetDeleteArray(&deleteArray_FS232ThcLcLTH2Func);
      instance.SetDestructor(&destruct_FS232ThcLcLTH2Func);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FS232Th::TH2Func*)
   {
      return GenerateInitInstanceLocal((::FS232Th::TH2Func*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::FS232Th::TH2Func*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FS232ThcLcLTH2Func_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FS232Th::TH2Func*)0x0)->GetClass();
      FS232ThcLcLTH2Func_TClassManip(theClass);
   return theClass;
   }

   static void FS232ThcLcLTH2Func_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_LambdaGenerator(void *p);
   static void deleteArray_LambdaGenerator(void *p);
   static void destruct_LambdaGenerator(void *p);
   static void streamer_LambdaGenerator(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LambdaGenerator*)
   {
      ::LambdaGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LambdaGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LambdaGenerator", ::LambdaGenerator::Class_Version(), "LambdaGenerator.h", 21,
                  typeid(::LambdaGenerator), DefineBehavior(ptr, ptr),
                  &::LambdaGenerator::Dictionary, isa_proxy, 16,
                  sizeof(::LambdaGenerator) );
      instance.SetDelete(&delete_LambdaGenerator);
      instance.SetDeleteArray(&deleteArray_LambdaGenerator);
      instance.SetDestructor(&destruct_LambdaGenerator);
      instance.SetStreamerFunc(&streamer_LambdaGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LambdaGenerator*)
   {
      return GenerateInitInstanceLocal((::LambdaGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::LambdaGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *Eta2C_Dictionary();
   static void Eta2C_TClassManip(TClass*);
   static void delete_Eta2C(void *p);
   static void deleteArray_Eta2C(void *p);
   static void destruct_Eta2C(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Eta2C*)
   {
      ::Eta2C *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Eta2C),0);
      static ::ROOT::TGenericClassInfo 
         instance("Eta2C", "Eta2C.h", 9,
                  typeid(::Eta2C), DefineBehavior(ptr, ptr),
                  &Eta2C_Dictionary, isa_proxy, 0,
                  sizeof(::Eta2C) );
      instance.SetDelete(&delete_Eta2C);
      instance.SetDeleteArray(&deleteArray_Eta2C);
      instance.SetDestructor(&destruct_Eta2C);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Eta2C*)
   {
      return GenerateInitInstanceLocal((::Eta2C*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Eta2C*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Eta2C_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Eta2C*)0x0)->GetClass();
      Eta2C_TClassManip(theClass);
   return theClass;
   }

   static void Eta2C_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_EtaDbFiller2C(void *p);
   static void deleteArray_EtaDbFiller2C(void *p);
   static void destruct_EtaDbFiller2C(void *p);
   static void streamer_EtaDbFiller2C(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EtaDbFiller2C*)
   {
      ::EtaDbFiller2C *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EtaDbFiller2C >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EtaDbFiller2C", ::EtaDbFiller2C::Class_Version(), "EtaDbFiller2C.h", 38,
                  typeid(::EtaDbFiller2C), DefineBehavior(ptr, ptr),
                  &::EtaDbFiller2C::Dictionary, isa_proxy, 16,
                  sizeof(::EtaDbFiller2C) );
      instance.SetDelete(&delete_EtaDbFiller2C);
      instance.SetDeleteArray(&deleteArray_EtaDbFiller2C);
      instance.SetDestructor(&destruct_EtaDbFiller2C);
      instance.SetStreamerFunc(&streamer_EtaDbFiller2C);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EtaDbFiller2C*)
   {
      return GenerateInitInstanceLocal((::EtaDbFiller2C*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EtaDbFiller2C*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *AngMarginalEta2C_Dictionary();
   static void AngMarginalEta2C_TClassManip(TClass*);
   static void delete_AngMarginalEta2C(void *p);
   static void deleteArray_AngMarginalEta2C(void *p);
   static void destruct_AngMarginalEta2C(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngMarginalEta2C*)
   {
      ::AngMarginalEta2C *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AngMarginalEta2C),0);
      static ::ROOT::TGenericClassInfo 
         instance("AngMarginalEta2C", "AngMarginalEta2C.h", 17,
                  typeid(::AngMarginalEta2C), DefineBehavior(ptr, ptr),
                  &AngMarginalEta2C_Dictionary, isa_proxy, 0,
                  sizeof(::AngMarginalEta2C) );
      instance.SetDelete(&delete_AngMarginalEta2C);
      instance.SetDeleteArray(&deleteArray_AngMarginalEta2C);
      instance.SetDestructor(&destruct_AngMarginalEta2C);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngMarginalEta2C*)
   {
      return GenerateInitInstanceLocal((::AngMarginalEta2C*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngMarginalEta2C*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AngMarginalEta2C_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AngMarginalEta2C*)0x0)->GetClass();
      AngMarginalEta2C_TClassManip(theClass);
   return theClass;
   }

   static void AngMarginalEta2C_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_SolidAnglePDFGeneratorC(void *p);
   static void deleteArray_SolidAnglePDFGeneratorC(void *p);
   static void destruct_SolidAnglePDFGeneratorC(void *p);
   static void streamer_SolidAnglePDFGeneratorC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SolidAnglePDFGeneratorC*)
   {
      ::SolidAnglePDFGeneratorC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SolidAnglePDFGeneratorC >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SolidAnglePDFGeneratorC", ::SolidAnglePDFGeneratorC::Class_Version(), "SolidAnglePDFGeneratorC.h", 21,
                  typeid(::SolidAnglePDFGeneratorC), DefineBehavior(ptr, ptr),
                  &::SolidAnglePDFGeneratorC::Dictionary, isa_proxy, 16,
                  sizeof(::SolidAnglePDFGeneratorC) );
      instance.SetDelete(&delete_SolidAnglePDFGeneratorC);
      instance.SetDeleteArray(&deleteArray_SolidAnglePDFGeneratorC);
      instance.SetDestructor(&destruct_SolidAnglePDFGeneratorC);
      instance.SetStreamerFunc(&streamer_SolidAnglePDFGeneratorC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SolidAnglePDFGeneratorC*)
   {
      return GenerateInitInstanceLocal((::SolidAnglePDFGeneratorC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SolidAnglePDFGeneratorC*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_YieldGenerator(void *p);
   static void deleteArray_YieldGenerator(void *p);
   static void destruct_YieldGenerator(void *p);
   static void streamer_YieldGenerator(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::YieldGenerator*)
   {
      ::YieldGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::YieldGenerator >(0);
      static ::ROOT::TGenericClassInfo 
         instance("YieldGenerator", ::YieldGenerator::Class_Version(), "YieldGenerator.h", 22,
                  typeid(::YieldGenerator), DefineBehavior(ptr, ptr),
                  &::YieldGenerator::Dictionary, isa_proxy, 16,
                  sizeof(::YieldGenerator) );
      instance.SetDelete(&delete_YieldGenerator);
      instance.SetDeleteArray(&deleteArray_YieldGenerator);
      instance.SetDestructor(&destruct_YieldGenerator);
      instance.SetStreamerFunc(&streamer_YieldGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::YieldGenerator*)
   {
      return GenerateInitInstanceLocal((::YieldGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::YieldGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_RatioVsOrigin(void *p);
   static void deleteArray_RatioVsOrigin(void *p);
   static void destruct_RatioVsOrigin(void *p);
   static void streamer_RatioVsOrigin(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RatioVsOrigin*)
   {
      ::RatioVsOrigin *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RatioVsOrigin >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RatioVsOrigin", ::RatioVsOrigin::Class_Version(), "RatioVsOrigin.h", 23,
                  typeid(::RatioVsOrigin), DefineBehavior(ptr, ptr),
                  &::RatioVsOrigin::Dictionary, isa_proxy, 16,
                  sizeof(::RatioVsOrigin) );
      instance.SetDelete(&delete_RatioVsOrigin);
      instance.SetDeleteArray(&deleteArray_RatioVsOrigin);
      instance.SetDestructor(&destruct_RatioVsOrigin);
      instance.SetStreamerFunc(&streamer_RatioVsOrigin);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RatioVsOrigin*)
   {
      return GenerateInitInstanceLocal((::RatioVsOrigin*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RatioVsOrigin*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_EnergyHistSorted(void *p);
   static void deleteArray_EnergyHistSorted(void *p);
   static void destruct_EnergyHistSorted(void *p);
   static void streamer_EnergyHistSorted(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnergyHistSorted*)
   {
      ::EnergyHistSorted *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EnergyHistSorted >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EnergyHistSorted", ::EnergyHistSorted::Class_Version(), "EnergyHistSorted.h", 23,
                  typeid(::EnergyHistSorted), DefineBehavior(ptr, ptr),
                  &::EnergyHistSorted::Dictionary, isa_proxy, 16,
                  sizeof(::EnergyHistSorted) );
      instance.SetDelete(&delete_EnergyHistSorted);
      instance.SetDeleteArray(&deleteArray_EnergyHistSorted);
      instance.SetDestructor(&destruct_EnergyHistSorted);
      instance.SetStreamerFunc(&streamer_EnergyHistSorted);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnergyHistSorted*)
   {
      return GenerateInitInstanceLocal((::EnergyHistSorted*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EnergyHistSorted*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TSelectorInsert::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSelectorInsert::Class_Name()
{
   return "TSelectorInsert";
}

//______________________________________________________________________________
const char *TSelectorInsert::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSelectorInsert*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSelectorInsert::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSelectorInsert*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSelectorInsert::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSelectorInsert*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSelectorInsert::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSelectorInsert*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SiStripGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SiStripGenerator::Class_Name()
{
   return "SiStripGenerator";
}

//______________________________________________________________________________
const char *SiStripGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SiStripGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SiStripGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SiStripGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SiStripGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SiStripGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SiStripGenerator::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SiStripGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr AngleAverager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *AngleAverager::Class_Name()
{
   return "AngleAverager";
}

//______________________________________________________________________________
const char *AngleAverager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngleAverager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int AngleAverager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::AngleAverager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AngleAverager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngleAverager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AngleAverager::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::AngleAverager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EnergyHist::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EnergyHist::Class_Name()
{
   return "EnergyHist";
}

//______________________________________________________________________________
const char *EnergyHist::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EnergyHist*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EnergyHist::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EnergyHist*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EnergyHist::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EnergyHist*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EnergyHist::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EnergyHist*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Analyzer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Analyzer::Class_Name()
{
   return "Analyzer";
}

//______________________________________________________________________________
const char *Analyzer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analyzer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Analyzer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Analyzer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Analyzer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analyzer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Analyzer::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Analyzer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LambdaGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LambdaGenerator::Class_Name()
{
   return "LambdaGenerator";
}

//______________________________________________________________________________
const char *LambdaGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LambdaGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LambdaGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LambdaGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LambdaGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LambdaGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LambdaGenerator::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LambdaGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EtaDbFiller2C::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EtaDbFiller2C::Class_Name()
{
   return "EtaDbFiller2C";
}

//______________________________________________________________________________
const char *EtaDbFiller2C::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EtaDbFiller2C*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EtaDbFiller2C::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EtaDbFiller2C*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EtaDbFiller2C::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EtaDbFiller2C*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EtaDbFiller2C::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EtaDbFiller2C*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SolidAnglePDFGeneratorC::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SolidAnglePDFGeneratorC::Class_Name()
{
   return "SolidAnglePDFGeneratorC";
}

//______________________________________________________________________________
const char *SolidAnglePDFGeneratorC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SolidAnglePDFGeneratorC*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SolidAnglePDFGeneratorC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SolidAnglePDFGeneratorC*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SolidAnglePDFGeneratorC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SolidAnglePDFGeneratorC*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SolidAnglePDFGeneratorC::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SolidAnglePDFGeneratorC*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr YieldGenerator::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *YieldGenerator::Class_Name()
{
   return "YieldGenerator";
}

//______________________________________________________________________________
const char *YieldGenerator::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::YieldGenerator*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int YieldGenerator::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::YieldGenerator*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *YieldGenerator::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::YieldGenerator*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *YieldGenerator::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::YieldGenerator*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RatioVsOrigin::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RatioVsOrigin::Class_Name()
{
   return "RatioVsOrigin";
}

//______________________________________________________________________________
const char *RatioVsOrigin::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RatioVsOrigin*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RatioVsOrigin::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RatioVsOrigin*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RatioVsOrigin::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RatioVsOrigin*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RatioVsOrigin::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RatioVsOrigin*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EnergyHistSorted::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EnergyHistSorted::Class_Name()
{
   return "EnergyHistSorted";
}

//______________________________________________________________________________
const char *EnergyHistSorted::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EnergyHistSorted*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EnergyHistSorted::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EnergyHistSorted*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EnergyHistSorted::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EnergyHistSorted*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EnergyHistSorted::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EnergyHistSorted*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TSelectorInsert::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSelectorInsert.

   ::Error("TSelectorInsert::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TSelectorInsert(void *p) {
      delete ((::TSelectorInsert*)p);
   }
   static void deleteArray_TSelectorInsert(void *p) {
      delete [] ((::TSelectorInsert*)p);
   }
   static void destruct_TSelectorInsert(void *p) {
      typedef ::TSelectorInsert current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TSelectorInsert(TBuffer &buf, void *obj) {
      ((::TSelectorInsert*)obj)->::TSelectorInsert::Streamer(buf);
   }
} // end of namespace ROOT for class ::TSelectorInsert

//______________________________________________________________________________
void SiStripGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class SiStripGenerator.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SiStripGenerator(void *p) {
      delete ((::SiStripGenerator*)p);
   }
   static void deleteArray_SiStripGenerator(void *p) {
      delete [] ((::SiStripGenerator*)p);
   }
   static void destruct_SiStripGenerator(void *p) {
      typedef ::SiStripGenerator current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SiStripGenerator(TBuffer &buf, void *obj) {
      ((::SiStripGenerator*)obj)->::SiStripGenerator::Streamer(buf);
   }
} // end of namespace ROOT for class ::SiStripGenerator

//______________________________________________________________________________
void AngleAverager::Streamer(TBuffer &R__b)
{
   // Stream an object of class AngleAverager.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AngleAverager(void *p) {
      delete ((::AngleAverager*)p);
   }
   static void deleteArray_AngleAverager(void *p) {
      delete [] ((::AngleAverager*)p);
   }
   static void destruct_AngleAverager(void *p) {
      typedef ::AngleAverager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_AngleAverager(TBuffer &buf, void *obj) {
      ((::AngleAverager*)obj)->::AngleAverager::Streamer(buf);
   }
} // end of namespace ROOT for class ::AngleAverager

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SolidAngleComputer(void *p) {
      delete ((::SolidAngleComputer*)p);
   }
   static void deleteArray_SolidAngleComputer(void *p) {
      delete [] ((::SolidAngleComputer*)p);
   }
   static void destruct_SolidAngleComputer(void *p) {
      typedef ::SolidAngleComputer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SolidAngleComputer

//______________________________________________________________________________
void EnergyHist::Streamer(TBuffer &R__b)
{
   // Stream an object of class EnergyHist.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EnergyHist(void *p) {
      delete ((::EnergyHist*)p);
   }
   static void deleteArray_EnergyHist(void *p) {
      delete [] ((::EnergyHist*)p);
   }
   static void destruct_EnergyHist(void *p) {
      typedef ::EnergyHist current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EnergyHist(TBuffer &buf, void *obj) {
      ((::EnergyHist*)obj)->::EnergyHist::Streamer(buf);
   }
} // end of namespace ROOT for class ::EnergyHist

namespace ROOT {
   // Wrappers around operator new
   static void *new_TTreeProxy(void *p) {
      return  p ? new(p) ::TTreeProxy : new ::TTreeProxy;
   }
   static void *newArray_TTreeProxy(Long_t nElements, void *p) {
      return p ? new(p) ::TTreeProxy[nElements] : new ::TTreeProxy[nElements];
   }
   // Wrapper around operator delete
   static void delete_TTreeProxy(void *p) {
      delete ((::TTreeProxy*)p);
   }
   static void deleteArray_TTreeProxy(void *p) {
      delete [] ((::TTreeProxy*)p);
   }
   static void destruct_TTreeProxy(void *p) {
      typedef ::TTreeProxy current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TTreeProxy

//______________________________________________________________________________
void Analyzer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Analyzer.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Analyzer(void *p) {
      return  p ? new(p) ::Analyzer : new ::Analyzer;
   }
   static void *newArray_Analyzer(Long_t nElements, void *p) {
      return p ? new(p) ::Analyzer[nElements] : new ::Analyzer[nElements];
   }
   // Wrapper around operator delete
   static void delete_Analyzer(void *p) {
      delete ((::Analyzer*)p);
   }
   static void deleteArray_Analyzer(void *p) {
      delete [] ((::Analyzer*)p);
   }
   static void destruct_Analyzer(void *p) {
      typedef ::Analyzer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Analyzer(TBuffer &buf, void *obj) {
      ((::Analyzer*)obj)->::Analyzer::Streamer(buf);
   }
} // end of namespace ROOT for class ::Analyzer

namespace ROOT {
   // Wrapper around operator delete
   static void delete_FS232ThcLcLTH2Func(void *p) {
      delete ((::FS232Th::TH2Func*)p);
   }
   static void deleteArray_FS232ThcLcLTH2Func(void *p) {
      delete [] ((::FS232Th::TH2Func*)p);
   }
   static void destruct_FS232ThcLcLTH2Func(void *p) {
      typedef ::FS232Th::TH2Func current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FS232Th::TH2Func

//______________________________________________________________________________
void LambdaGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class LambdaGenerator.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_LambdaGenerator(void *p) {
      delete ((::LambdaGenerator*)p);
   }
   static void deleteArray_LambdaGenerator(void *p) {
      delete [] ((::LambdaGenerator*)p);
   }
   static void destruct_LambdaGenerator(void *p) {
      typedef ::LambdaGenerator current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_LambdaGenerator(TBuffer &buf, void *obj) {
      ((::LambdaGenerator*)obj)->::LambdaGenerator::Streamer(buf);
   }
} // end of namespace ROOT for class ::LambdaGenerator

namespace ROOT {
   // Wrapper around operator delete
   static void delete_Eta2C(void *p) {
      delete ((::Eta2C*)p);
   }
   static void deleteArray_Eta2C(void *p) {
      delete [] ((::Eta2C*)p);
   }
   static void destruct_Eta2C(void *p) {
      typedef ::Eta2C current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Eta2C

//______________________________________________________________________________
void EtaDbFiller2C::Streamer(TBuffer &R__b)
{
   // Stream an object of class EtaDbFiller2C.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EtaDbFiller2C(void *p) {
      delete ((::EtaDbFiller2C*)p);
   }
   static void deleteArray_EtaDbFiller2C(void *p) {
      delete [] ((::EtaDbFiller2C*)p);
   }
   static void destruct_EtaDbFiller2C(void *p) {
      typedef ::EtaDbFiller2C current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EtaDbFiller2C(TBuffer &buf, void *obj) {
      ((::EtaDbFiller2C*)obj)->::EtaDbFiller2C::Streamer(buf);
   }
} // end of namespace ROOT for class ::EtaDbFiller2C

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AngMarginalEta2C(void *p) {
      delete ((::AngMarginalEta2C*)p);
   }
   static void deleteArray_AngMarginalEta2C(void *p) {
      delete [] ((::AngMarginalEta2C*)p);
   }
   static void destruct_AngMarginalEta2C(void *p) {
      typedef ::AngMarginalEta2C current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AngMarginalEta2C

//______________________________________________________________________________
void SolidAnglePDFGeneratorC::Streamer(TBuffer &R__b)
{
   // Stream an object of class SolidAnglePDFGeneratorC.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SolidAnglePDFGeneratorC(void *p) {
      delete ((::SolidAnglePDFGeneratorC*)p);
   }
   static void deleteArray_SolidAnglePDFGeneratorC(void *p) {
      delete [] ((::SolidAnglePDFGeneratorC*)p);
   }
   static void destruct_SolidAnglePDFGeneratorC(void *p) {
      typedef ::SolidAnglePDFGeneratorC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SolidAnglePDFGeneratorC(TBuffer &buf, void *obj) {
      ((::SolidAnglePDFGeneratorC*)obj)->::SolidAnglePDFGeneratorC::Streamer(buf);
   }
} // end of namespace ROOT for class ::SolidAnglePDFGeneratorC

//______________________________________________________________________________
void YieldGenerator::Streamer(TBuffer &R__b)
{
   // Stream an object of class YieldGenerator.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_YieldGenerator(void *p) {
      delete ((::YieldGenerator*)p);
   }
   static void deleteArray_YieldGenerator(void *p) {
      delete [] ((::YieldGenerator*)p);
   }
   static void destruct_YieldGenerator(void *p) {
      typedef ::YieldGenerator current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_YieldGenerator(TBuffer &buf, void *obj) {
      ((::YieldGenerator*)obj)->::YieldGenerator::Streamer(buf);
   }
} // end of namespace ROOT for class ::YieldGenerator

//______________________________________________________________________________
void RatioVsOrigin::Streamer(TBuffer &R__b)
{
   // Stream an object of class RatioVsOrigin.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RatioVsOrigin(void *p) {
      delete ((::RatioVsOrigin*)p);
   }
   static void deleteArray_RatioVsOrigin(void *p) {
      delete [] ((::RatioVsOrigin*)p);
   }
   static void destruct_RatioVsOrigin(void *p) {
      typedef ::RatioVsOrigin current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_RatioVsOrigin(TBuffer &buf, void *obj) {
      ((::RatioVsOrigin*)obj)->::RatioVsOrigin::Streamer(buf);
   }
} // end of namespace ROOT for class ::RatioVsOrigin

//______________________________________________________________________________
void EnergyHistSorted::Streamer(TBuffer &R__b)
{
   // Stream an object of class EnergyHistSorted.

   TSelectorInsert::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EnergyHistSorted(void *p) {
      delete ((::EnergyHistSorted*)p);
   }
   static void deleteArray_EnergyHistSorted(void *p) {
      delete [] ((::EnergyHistSorted*)p);
   }
   static void destruct_EnergyHistSorted(void *p) {
      typedef ::EnergyHistSorted current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EnergyHistSorted(TBuffer &buf, void *obj) {
      ((::EnergyHistSorted*)obj)->::EnergyHistSorted::Streamer(buf);
   }
} // end of namespace ROOT for class ::EnergyHistSorted

namespace {
  void TriggerDictionaryInitialization_dictFragSimAnalyzer_Impl() {
    static const char* headers[] = {
"TSelectorInsert.h",
"SiStripGenerator.h",
"AngleAverager.h",
"SolidAngleComputer.h",
"EnergyHist.h",
"TTreeProxy.h",
"Analyzer.h",
"TH2Func.h",
"LambdaGenerator.h",
"Eta2C.h",
"EtaDbFiller2C.h",
"AngMarginalEta2C.h",
"SolidAnglePDFGeneratorC.h",
"YieldGenerator.h",
"RatioVsOrigin.h",
"EnergyHistSorted.h",
0
    };
    static const char* includePaths[] = {
"-I/home/jeromy/Code/include",
"/home/jeromy/Exps/G4Work/FragSim232Th/include",
"/usr/local/geant4/geant4.9.5.p01/include/Geant4",
"/usr/local/CLHEP/include",
"/home/jeromy/Code/include",
"/home/jeromy/Exps/G4Work/FragSim232Th/include",
"/usr/local/root/root-6.02.03/include",
"/home/jeromy/Exps/G4Work/FragSim232Th/analysis/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TSelectorInsert.h")))  TSelectorInsert;
class __attribute__((annotate("$clingAutoload$SiStripGenerator.h")))  SiStripGenerator;
class __attribute__((annotate("$clingAutoload$AngleAverager.h")))  AngleAverager;
class __attribute__((annotate("$clingAutoload$SolidAngleComputer.h")))  SolidAngleComputer;
class __attribute__((annotate("$clingAutoload$EnergyHist.h")))  EnergyHist;
class __attribute__((annotate("$clingAutoload$TSelectorInsert.h")))  TTreeProxy;
class __attribute__((annotate("$clingAutoload$Analyzer.h")))  Analyzer;
namespace FS232Th{class __attribute__((annotate("$clingAutoload$TH2Func.h")))  TH2Func;}
class __attribute__((annotate("$clingAutoload$LambdaGenerator.h")))  LambdaGenerator;
class __attribute__((annotate("$clingAutoload$Eta2C.h")))  Eta2C;
class __attribute__((annotate("$clingAutoload$EtaDbFiller2C.h")))  EtaDbFiller2C;
class __attribute__((annotate("$clingAutoload$AngMarginalEta2C.h")))  AngMarginalEta2C;
class __attribute__((annotate("$clingAutoload$SolidAnglePDFGeneratorC.h")))  SolidAnglePDFGeneratorC;
class __attribute__((annotate("$clingAutoload$YieldGenerator.h")))  YieldGenerator;
class __attribute__((annotate("$clingAutoload$RatioVsOrigin.h")))  RatioVsOrigin;
class __attribute__((annotate("$clingAutoload$EnergyHistSorted.h")))  EnergyHistSorted;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TSelectorInsert.h"
#include "SiStripGenerator.h"
#include "AngleAverager.h"
#include "SolidAngleComputer.h"
#include "EnergyHist.h"
#include "TTreeProxy.h"
#include "Analyzer.h"
#include "TH2Func.h"
#include "LambdaGenerator.h"
#include "Eta2C.h"
#include "EtaDbFiller2C.h"
#include "AngMarginalEta2C.h"
#include "SolidAnglePDFGeneratorC.h"
#include "YieldGenerator.h"
#include "RatioVsOrigin.h"
#include "EnergyHistSorted.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Analyzer", payloadCode, "@",
"AngMarginalEta2C", payloadCode, "@",
"AngleAverager", payloadCode, "@",
"EnergyHist", payloadCode, "@",
"EnergyHistSorted", payloadCode, "@",
"Eta2C", payloadCode, "@",
"EtaDbFiller2C", payloadCode, "@",
"FS232Th::TH2Func", payloadCode, "@",
"LambdaGenerator", payloadCode, "@",
"RatioVsOrigin", payloadCode, "@",
"SiStripGenerator", payloadCode, "@",
"SolidAngleComputer", payloadCode, "@",
"SolidAnglePDFGeneratorC", payloadCode, "@",
"TSelectorInsert", payloadCode, "@",
"TTreeProxy", payloadCode, "@",
"YieldGenerator", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictFragSimAnalyzer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictFragSimAnalyzer_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictFragSimAnalyzer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictFragSimAnalyzer() {
  TriggerDictionaryInitialization_dictFragSimAnalyzer_Impl();
}
