// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictAnalyzer

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
#include "ConfigEntry.h"
#include "ConfigManager.h"
#include "ProofPlot.h"
#include "PP2d.h"
#include "PPCoincMap.h"
#include "PPCoincMapRegions.h"
#include "SelectPPCoincMapRegions.h"
#include "PAnalysis.h"
#include "Visualizer.h"
#include "PFVisualizer.h"
#include "ScalerTool.h"
#include "SScalerTool.h"
#include "Comment.h"
#include "FNameManager.h"
#include "ExtensibleDb.h"
#include "DataElement.h"
#include "Utils.h"
#include "CombinationEngine.h"
#include "MultiplicityGroupGenerator.h"
#include "FluxCalculator.h"
#include "ADCRegionInspector.h"
#include "StripInfo.h"
#include "UtilTemplates.h"

// Header files passed via #pragma extra_include

namespace PAnalysis {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *PAnalysis_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("PAnalysis", 0 /*version*/, "PAnalysis.h", 15,
                     ::ROOT::DefineBehavior((void*)0,(void*)0),
                     &PAnalysis_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *PAnalysis_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_ConfigEntry(void *p = 0);
   static void *newArray_ConfigEntry(Long_t size, void *p);
   static void delete_ConfigEntry(void *p);
   static void deleteArray_ConfigEntry(void *p);
   static void destruct_ConfigEntry(void *p);
   static void streamer_ConfigEntry(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigEntry*)
   {
      ::ConfigEntry *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigEntry >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigEntry", ::ConfigEntry::Class_Version(), "ConfigEntry.h", 21,
                  typeid(::ConfigEntry), DefineBehavior(ptr, ptr),
                  &::ConfigEntry::Dictionary, isa_proxy, 16,
                  sizeof(::ConfigEntry) );
      instance.SetNew(&new_ConfigEntry);
      instance.SetNewArray(&newArray_ConfigEntry);
      instance.SetDelete(&delete_ConfigEntry);
      instance.SetDeleteArray(&deleteArray_ConfigEntry);
      instance.SetDestructor(&destruct_ConfigEntry);
      instance.SetStreamerFunc(&streamer_ConfigEntry);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigEntry*)
   {
      return GenerateInitInstanceLocal((::ConfigEntry*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ConfigEntry*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ConfigManager(void *p = 0);
   static void *newArray_ConfigManager(Long_t size, void *p);
   static void delete_ConfigManager(void *p);
   static void deleteArray_ConfigManager(void *p);
   static void destruct_ConfigManager(void *p);
   static void streamer_ConfigManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConfigManager*)
   {
      ::ConfigManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConfigManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConfigManager", ::ConfigManager::Class_Version(), "ConfigManager.h", 25,
                  typeid(::ConfigManager), DefineBehavior(ptr, ptr),
                  &::ConfigManager::Dictionary, isa_proxy, 16,
                  sizeof(::ConfigManager) );
      instance.SetNew(&new_ConfigManager);
      instance.SetNewArray(&newArray_ConfigManager);
      instance.SetDelete(&delete_ConfigManager);
      instance.SetDeleteArray(&deleteArray_ConfigManager);
      instance.SetDestructor(&destruct_ConfigManager);
      instance.SetStreamerFunc(&streamer_ConfigManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConfigManager*)
   {
      return GenerateInitInstanceLocal((::ConfigManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ConfigManager*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_ProofPlot(void *p);
   static void deleteArray_ProofPlot(void *p);
   static void destruct_ProofPlot(void *p);
   static void streamer_ProofPlot(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ProofPlot*)
   {
      ::ProofPlot *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ProofPlot >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ProofPlot", ::ProofPlot::Class_Version(), "ProofPlot.h", 27,
                  typeid(::ProofPlot), DefineBehavior(ptr, ptr),
                  &::ProofPlot::Dictionary, isa_proxy, 16,
                  sizeof(::ProofPlot) );
      instance.SetDelete(&delete_ProofPlot);
      instance.SetDeleteArray(&deleteArray_ProofPlot);
      instance.SetDestructor(&destruct_ProofPlot);
      instance.SetStreamerFunc(&streamer_ProofPlot);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ProofPlot*)
   {
      return GenerateInitInstanceLocal((::ProofPlot*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ProofPlot*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PP2d(void *p);
   static void deleteArray_PP2d(void *p);
   static void destruct_PP2d(void *p);
   static void streamer_PP2d(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PP2d*)
   {
      ::PP2d *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PP2d >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PP2d", ::PP2d::Class_Version(), "PP2d.h", 26,
                  typeid(::PP2d), DefineBehavior(ptr, ptr),
                  &::PP2d::Dictionary, isa_proxy, 16,
                  sizeof(::PP2d) );
      instance.SetDelete(&delete_PP2d);
      instance.SetDeleteArray(&deleteArray_PP2d);
      instance.SetDestructor(&destruct_PP2d);
      instance.SetStreamerFunc(&streamer_PP2d);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PP2d*)
   {
      return GenerateInitInstanceLocal((::PP2d*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PP2d*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PPCoincMap(void *p);
   static void deleteArray_PPCoincMap(void *p);
   static void destruct_PPCoincMap(void *p);
   static void streamer_PPCoincMap(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PPCoincMap*)
   {
      ::PPCoincMap *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PPCoincMap >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PPCoincMap", ::PPCoincMap::Class_Version(), "PPCoincMap.h", 27,
                  typeid(::PPCoincMap), DefineBehavior(ptr, ptr),
                  &::PPCoincMap::Dictionary, isa_proxy, 16,
                  sizeof(::PPCoincMap) );
      instance.SetDelete(&delete_PPCoincMap);
      instance.SetDeleteArray(&deleteArray_PPCoincMap);
      instance.SetDestructor(&destruct_PPCoincMap);
      instance.SetStreamerFunc(&streamer_PPCoincMap);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PPCoincMap*)
   {
      return GenerateInitInstanceLocal((::PPCoincMap*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PPCoincMap*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PPCoincMapRegions(void *p);
   static void deleteArray_PPCoincMapRegions(void *p);
   static void destruct_PPCoincMapRegions(void *p);
   static void streamer_PPCoincMapRegions(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PPCoincMapRegions*)
   {
      ::PPCoincMapRegions *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PPCoincMapRegions >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PPCoincMapRegions", ::PPCoincMapRegions::Class_Version(), "PPCoincMapRegions.h", 58,
                  typeid(::PPCoincMapRegions), DefineBehavior(ptr, ptr),
                  &::PPCoincMapRegions::Dictionary, isa_proxy, 16,
                  sizeof(::PPCoincMapRegions) );
      instance.SetDelete(&delete_PPCoincMapRegions);
      instance.SetDeleteArray(&deleteArray_PPCoincMapRegions);
      instance.SetDestructor(&destruct_PPCoincMapRegions);
      instance.SetStreamerFunc(&streamer_PPCoincMapRegions);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PPCoincMapRegions*)
   {
      return GenerateInitInstanceLocal((::PPCoincMapRegions*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PPCoincMapRegions*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_SelectPPCoincMapRegions(void *p);
   static void deleteArray_SelectPPCoincMapRegions(void *p);
   static void destruct_SelectPPCoincMapRegions(void *p);
   static void streamer_SelectPPCoincMapRegions(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SelectPPCoincMapRegions*)
   {
      ::SelectPPCoincMapRegions *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SelectPPCoincMapRegions >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SelectPPCoincMapRegions", ::SelectPPCoincMapRegions::Class_Version(), "SelectPPCoincMapRegions.h", 34,
                  typeid(::SelectPPCoincMapRegions), DefineBehavior(ptr, ptr),
                  &::SelectPPCoincMapRegions::Dictionary, isa_proxy, 16,
                  sizeof(::SelectPPCoincMapRegions) );
      instance.SetDelete(&delete_SelectPPCoincMapRegions);
      instance.SetDeleteArray(&deleteArray_SelectPPCoincMapRegions);
      instance.SetDestructor(&destruct_SelectPPCoincMapRegions);
      instance.SetStreamerFunc(&streamer_SelectPPCoincMapRegions);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SelectPPCoincMapRegions*)
   {
      return GenerateInitInstanceLocal((::SelectPPCoincMapRegions*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SelectPPCoincMapRegions*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Visualizer(void *p = 0);
   static void *newArray_Visualizer(Long_t size, void *p);
   static void delete_Visualizer(void *p);
   static void deleteArray_Visualizer(void *p);
   static void destruct_Visualizer(void *p);
   static void streamer_Visualizer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Visualizer*)
   {
      ::Visualizer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Visualizer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Visualizer", ::Visualizer::Class_Version(), "Visualizer.h", 23,
                  typeid(::Visualizer), DefineBehavior(ptr, ptr),
                  &::Visualizer::Dictionary, isa_proxy, 16,
                  sizeof(::Visualizer) );
      instance.SetNew(&new_Visualizer);
      instance.SetNewArray(&newArray_Visualizer);
      instance.SetDelete(&delete_Visualizer);
      instance.SetDeleteArray(&deleteArray_Visualizer);
      instance.SetDestructor(&destruct_Visualizer);
      instance.SetStreamerFunc(&streamer_Visualizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Visualizer*)
   {
      return GenerateInitInstanceLocal((::Visualizer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Visualizer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_PFVisualizer(void *p);
   static void deleteArray_PFVisualizer(void *p);
   static void destruct_PFVisualizer(void *p);
   static void streamer_PFVisualizer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PFVisualizer*)
   {
      ::PFVisualizer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PFVisualizer >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PFVisualizer", ::PFVisualizer::Class_Version(), "PFVisualizer.h", 17,
                  typeid(::PFVisualizer), DefineBehavior(ptr, ptr),
                  &::PFVisualizer::Dictionary, isa_proxy, 16,
                  sizeof(::PFVisualizer) );
      instance.SetDelete(&delete_PFVisualizer);
      instance.SetDeleteArray(&deleteArray_PFVisualizer);
      instance.SetDestructor(&destruct_PFVisualizer);
      instance.SetStreamerFunc(&streamer_PFVisualizer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PFVisualizer*)
   {
      return GenerateInitInstanceLocal((::PFVisualizer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PFVisualizer*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ScalerTool(void *p = 0);
   static void *newArray_ScalerTool(Long_t size, void *p);
   static void delete_ScalerTool(void *p);
   static void deleteArray_ScalerTool(void *p);
   static void destruct_ScalerTool(void *p);
   static void streamer_ScalerTool(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ScalerTool*)
   {
      ::ScalerTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ScalerTool >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ScalerTool", ::ScalerTool::Class_Version(), "ScalerTool.h", 23,
                  typeid(::ScalerTool), DefineBehavior(ptr, ptr),
                  &::ScalerTool::Dictionary, isa_proxy, 16,
                  sizeof(::ScalerTool) );
      instance.SetNew(&new_ScalerTool);
      instance.SetNewArray(&newArray_ScalerTool);
      instance.SetDelete(&delete_ScalerTool);
      instance.SetDeleteArray(&deleteArray_ScalerTool);
      instance.SetDestructor(&destruct_ScalerTool);
      instance.SetStreamerFunc(&streamer_ScalerTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ScalerTool*)
   {
      return GenerateInitInstanceLocal((::ScalerTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ScalerTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SScalerTool(void *p = 0);
   static void *newArray_SScalerTool(Long_t size, void *p);
   static void delete_SScalerTool(void *p);
   static void deleteArray_SScalerTool(void *p);
   static void destruct_SScalerTool(void *p);
   static void streamer_SScalerTool(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SScalerTool*)
   {
      ::SScalerTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SScalerTool >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SScalerTool", ::SScalerTool::Class_Version(), "SScalerTool.h", 17,
                  typeid(::SScalerTool), DefineBehavior(ptr, ptr),
                  &::SScalerTool::Dictionary, isa_proxy, 16,
                  sizeof(::SScalerTool) );
      instance.SetNew(&new_SScalerTool);
      instance.SetNewArray(&newArray_SScalerTool);
      instance.SetDelete(&delete_SScalerTool);
      instance.SetDeleteArray(&deleteArray_SScalerTool);
      instance.SetDestructor(&destruct_SScalerTool);
      instance.SetStreamerFunc(&streamer_SScalerTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SScalerTool*)
   {
      return GenerateInitInstanceLocal((::SScalerTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SScalerTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_FNameManager(void *p = 0);
   static void *newArray_FNameManager(Long_t size, void *p);
   static void delete_FNameManager(void *p);
   static void deleteArray_FNameManager(void *p);
   static void destruct_FNameManager(void *p);
   static void streamer_FNameManager(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FNameManager*)
   {
      ::FNameManager *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::FNameManager >(0);
      static ::ROOT::TGenericClassInfo 
         instance("FNameManager", ::FNameManager::Class_Version(), "FNameManager.h", 14,
                  typeid(::FNameManager), DefineBehavior(ptr, ptr),
                  &::FNameManager::Dictionary, isa_proxy, 16,
                  sizeof(::FNameManager) );
      instance.SetNew(&new_FNameManager);
      instance.SetNewArray(&newArray_FNameManager);
      instance.SetDelete(&delete_FNameManager);
      instance.SetDeleteArray(&deleteArray_FNameManager);
      instance.SetDestructor(&destruct_FNameManager);
      instance.SetStreamerFunc(&streamer_FNameManager);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FNameManager*)
   {
      return GenerateInitInstanceLocal((::FNameManager*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::FNameManager*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Comment(void *p = 0);
   static void *newArray_Comment(Long_t size, void *p);
   static void delete_Comment(void *p);
   static void deleteArray_Comment(void *p);
   static void destruct_Comment(void *p);
   static void streamer_Comment(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Comment*)
   {
      ::Comment *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Comment >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Comment", ::Comment::Class_Version(), "Comment.h", 15,
                  typeid(::Comment), DefineBehavior(ptr, ptr),
                  &::Comment::Dictionary, isa_proxy, 16,
                  sizeof(::Comment) );
      instance.SetNew(&new_Comment);
      instance.SetNewArray(&newArray_Comment);
      instance.SetDelete(&delete_Comment);
      instance.SetDeleteArray(&deleteArray_Comment);
      instance.SetDestructor(&destruct_Comment);
      instance.SetStreamerFunc(&streamer_Comment);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Comment*)
   {
      return GenerateInitInstanceLocal((::Comment*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Comment*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ExtensibleDb(void *p = 0);
   static void *newArray_ExtensibleDb(Long_t size, void *p);
   static void delete_ExtensibleDb(void *p);
   static void deleteArray_ExtensibleDb(void *p);
   static void destruct_ExtensibleDb(void *p);
   static void streamer_ExtensibleDb(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ExtensibleDb*)
   {
      ::ExtensibleDb *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ExtensibleDb >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ExtensibleDb", ::ExtensibleDb::Class_Version(), "ExtensibleDb.h", 33,
                  typeid(::ExtensibleDb), DefineBehavior(ptr, ptr),
                  &::ExtensibleDb::Dictionary, isa_proxy, 16,
                  sizeof(::ExtensibleDb) );
      instance.SetNew(&new_ExtensibleDb);
      instance.SetNewArray(&newArray_ExtensibleDb);
      instance.SetDelete(&delete_ExtensibleDb);
      instance.SetDeleteArray(&deleteArray_ExtensibleDb);
      instance.SetDestructor(&destruct_ExtensibleDb);
      instance.SetStreamerFunc(&streamer_ExtensibleDb);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ExtensibleDb*)
   {
      return GenerateInitInstanceLocal((::ExtensibleDb*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ExtensibleDb*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_DataElement(void *p);
   static void deleteArray_DataElement(void *p);
   static void destruct_DataElement(void *p);
   static void streamer_DataElement(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DataElement*)
   {
      ::DataElement *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DataElement >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DataElement", ::DataElement::Class_Version(), "DataElement.h", 27,
                  typeid(::DataElement), DefineBehavior(ptr, ptr),
                  &::DataElement::Dictionary, isa_proxy, 16,
                  sizeof(::DataElement) );
      instance.SetDelete(&delete_DataElement);
      instance.SetDeleteArray(&deleteArray_DataElement);
      instance.SetDestructor(&destruct_DataElement);
      instance.SetStreamerFunc(&streamer_DataElement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DataElement*)
   {
      return GenerateInitInstanceLocal((::DataElement*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DataElement*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *CombinationEngine_Dictionary();
   static void CombinationEngine_TClassManip(TClass*);
   static void delete_CombinationEngine(void *p);
   static void deleteArray_CombinationEngine(void *p);
   static void destruct_CombinationEngine(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CombinationEngine*)
   {
      ::CombinationEngine *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CombinationEngine),0);
      static ::ROOT::TGenericClassInfo 
         instance("CombinationEngine", "CombinationEngine.h", 11,
                  typeid(::CombinationEngine), DefineBehavior(ptr, ptr),
                  &CombinationEngine_Dictionary, isa_proxy, 0,
                  sizeof(::CombinationEngine) );
      instance.SetDelete(&delete_CombinationEngine);
      instance.SetDeleteArray(&deleteArray_CombinationEngine);
      instance.SetDestructor(&destruct_CombinationEngine);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CombinationEngine*)
   {
      return GenerateInitInstanceLocal((::CombinationEngine*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CombinationEngine*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CombinationEngine_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CombinationEngine*)0x0)->GetClass();
      CombinationEngine_TClassManip(theClass);
   return theClass;
   }

   static void CombinationEngine_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *MultiplicityGroupGenerator_Dictionary();
   static void MultiplicityGroupGenerator_TClassManip(TClass*);
   static void delete_MultiplicityGroupGenerator(void *p);
   static void deleteArray_MultiplicityGroupGenerator(void *p);
   static void destruct_MultiplicityGroupGenerator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MultiplicityGroupGenerator*)
   {
      ::MultiplicityGroupGenerator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MultiplicityGroupGenerator),0);
      static ::ROOT::TGenericClassInfo 
         instance("MultiplicityGroupGenerator", "MultiplicityGroupGenerator.h", 8,
                  typeid(::MultiplicityGroupGenerator), DefineBehavior(ptr, ptr),
                  &MultiplicityGroupGenerator_Dictionary, isa_proxy, 0,
                  sizeof(::MultiplicityGroupGenerator) );
      instance.SetDelete(&delete_MultiplicityGroupGenerator);
      instance.SetDeleteArray(&deleteArray_MultiplicityGroupGenerator);
      instance.SetDestructor(&destruct_MultiplicityGroupGenerator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MultiplicityGroupGenerator*)
   {
      return GenerateInitInstanceLocal((::MultiplicityGroupGenerator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MultiplicityGroupGenerator*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *MultiplicityGroupGenerator_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::MultiplicityGroupGenerator*)0x0)->GetClass();
      MultiplicityGroupGenerator_TClassManip(theClass);
   return theClass;
   }

   static void MultiplicityGroupGenerator_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FluxCalculator_Dictionary();
   static void FluxCalculator_TClassManip(TClass*);
   static void delete_FluxCalculator(void *p);
   static void deleteArray_FluxCalculator(void *p);
   static void destruct_FluxCalculator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FluxCalculator*)
   {
      ::FluxCalculator *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FluxCalculator),0);
      static ::ROOT::TGenericClassInfo 
         instance("FluxCalculator", "FluxCalculator.h", 20,
                  typeid(::FluxCalculator), DefineBehavior(ptr, ptr),
                  &FluxCalculator_Dictionary, isa_proxy, 0,
                  sizeof(::FluxCalculator) );
      instance.SetDelete(&delete_FluxCalculator);
      instance.SetDeleteArray(&deleteArray_FluxCalculator);
      instance.SetDestructor(&destruct_FluxCalculator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FluxCalculator*)
   {
      return GenerateInitInstanceLocal((::FluxCalculator*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::FluxCalculator*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FluxCalculator_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FluxCalculator*)0x0)->GetClass();
      FluxCalculator_TClassManip(theClass);
   return theClass;
   }

   static void FluxCalculator_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_ADCRegionInspector(void *p);
   static void deleteArray_ADCRegionInspector(void *p);
   static void destruct_ADCRegionInspector(void *p);
   static void streamer_ADCRegionInspector(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ADCRegionInspector*)
   {
      ::ADCRegionInspector *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ADCRegionInspector >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ADCRegionInspector", ::ADCRegionInspector::Class_Version(), "ADCRegionInspector.h", 31,
                  typeid(::ADCRegionInspector), DefineBehavior(ptr, ptr),
                  &::ADCRegionInspector::Dictionary, isa_proxy, 16,
                  sizeof(::ADCRegionInspector) );
      instance.SetDelete(&delete_ADCRegionInspector);
      instance.SetDeleteArray(&deleteArray_ADCRegionInspector);
      instance.SetDestructor(&destruct_ADCRegionInspector);
      instance.SetStreamerFunc(&streamer_ADCRegionInspector);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ADCRegionInspector*)
   {
      return GenerateInitInstanceLocal((::ADCRegionInspector*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ADCRegionInspector*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *StripInfo_Dictionary();
   static void StripInfo_TClassManip(TClass*);
   static void *new_StripInfo(void *p = 0);
   static void *newArray_StripInfo(Long_t size, void *p);
   static void delete_StripInfo(void *p);
   static void deleteArray_StripInfo(void *p);
   static void destruct_StripInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StripInfo*)
   {
      ::StripInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StripInfo),0);
      static ::ROOT::TGenericClassInfo 
         instance("StripInfo", "StripInfo.h", 11,
                  typeid(::StripInfo), DefineBehavior(ptr, ptr),
                  &StripInfo_Dictionary, isa_proxy, 0,
                  sizeof(::StripInfo) );
      instance.SetNew(&new_StripInfo);
      instance.SetNewArray(&newArray_StripInfo);
      instance.SetDelete(&delete_StripInfo);
      instance.SetDeleteArray(&deleteArray_StripInfo);
      instance.SetDestructor(&destruct_StripInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StripInfo*)
   {
      return GenerateInitInstanceLocal((::StripInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StripInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StripInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StripInfo*)0x0)->GetClass();
      StripInfo_TClassManip(theClass);
   return theClass;
   }

   static void StripInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *StripDb_Dictionary();
   static void StripDb_TClassManip(TClass*);
   static void delete_StripDb(void *p);
   static void deleteArray_StripDb(void *p);
   static void destruct_StripDb(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StripDb*)
   {
      ::StripDb *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::StripDb),0);
      static ::ROOT::TGenericClassInfo 
         instance("StripDb", "StripInfo.h", 68,
                  typeid(::StripDb), DefineBehavior(ptr, ptr),
                  &StripDb_Dictionary, isa_proxy, 0,
                  sizeof(::StripDb) );
      instance.SetDelete(&delete_StripDb);
      instance.SetDeleteArray(&deleteArray_StripDb);
      instance.SetDestructor(&destruct_StripDb);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StripDb*)
   {
      return GenerateInitInstanceLocal((::StripDb*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::StripDb*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *StripDb_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::StripDb*)0x0)->GetClass();
      StripDb_TClassManip(theClass);
   return theClass;
   }

   static void StripDb_TClassManip(TClass* ){
   }

} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr ConfigEntry::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ConfigEntry::Class_Name()
{
   return "ConfigEntry";
}

//______________________________________________________________________________
const char *ConfigEntry::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigEntry*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ConfigEntry::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigEntry*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConfigEntry::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigEntry*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConfigEntry::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigEntry*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ConfigManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ConfigManager::Class_Name()
{
   return "ConfigManager";
}

//______________________________________________________________________________
const char *ConfigManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ConfigManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConfigManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConfigManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConfigManager::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConfigManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ProofPlot::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ProofPlot::Class_Name()
{
   return "ProofPlot";
}

//______________________________________________________________________________
const char *ProofPlot::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ProofPlot*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ProofPlot::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ProofPlot*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ProofPlot::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ProofPlot*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ProofPlot::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ProofPlot*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PP2d::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PP2d::Class_Name()
{
   return "PP2d";
}

//______________________________________________________________________________
const char *PP2d::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PP2d*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PP2d::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PP2d*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PP2d::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PP2d*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PP2d::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PP2d*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PPCoincMap::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PPCoincMap::Class_Name()
{
   return "PPCoincMap";
}

//______________________________________________________________________________
const char *PPCoincMap::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMap*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PPCoincMap::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMap*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PPCoincMap::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMap*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PPCoincMap::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMap*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PPCoincMapRegions::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PPCoincMapRegions::Class_Name()
{
   return "PPCoincMapRegions";
}

//______________________________________________________________________________
const char *PPCoincMapRegions::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMapRegions*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PPCoincMapRegions::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMapRegions*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PPCoincMapRegions::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMapRegions*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PPCoincMapRegions::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PPCoincMapRegions*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SelectPPCoincMapRegions::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SelectPPCoincMapRegions::Class_Name()
{
   return "SelectPPCoincMapRegions";
}

//______________________________________________________________________________
const char *SelectPPCoincMapRegions::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SelectPPCoincMapRegions*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SelectPPCoincMapRegions::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SelectPPCoincMapRegions*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SelectPPCoincMapRegions::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SelectPPCoincMapRegions*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SelectPPCoincMapRegions::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SelectPPCoincMapRegions*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Visualizer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Visualizer::Class_Name()
{
   return "Visualizer";
}

//______________________________________________________________________________
const char *Visualizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Visualizer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Visualizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Visualizer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Visualizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Visualizer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Visualizer::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Visualizer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PFVisualizer::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PFVisualizer::Class_Name()
{
   return "PFVisualizer";
}

//______________________________________________________________________________
const char *PFVisualizer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PFVisualizer*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PFVisualizer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PFVisualizer*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PFVisualizer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PFVisualizer*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PFVisualizer::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PFVisualizer*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ScalerTool::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ScalerTool::Class_Name()
{
   return "ScalerTool";
}

//______________________________________________________________________________
const char *ScalerTool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ScalerTool*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ScalerTool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ScalerTool*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ScalerTool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ScalerTool*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ScalerTool::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ScalerTool*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SScalerTool::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SScalerTool::Class_Name()
{
   return "SScalerTool";
}

//______________________________________________________________________________
const char *SScalerTool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SScalerTool*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SScalerTool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SScalerTool*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SScalerTool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SScalerTool*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SScalerTool::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SScalerTool*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr FNameManager::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *FNameManager::Class_Name()
{
   return "FNameManager";
}

//______________________________________________________________________________
const char *FNameManager::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FNameManager*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int FNameManager::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::FNameManager*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FNameManager::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FNameManager*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FNameManager::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::FNameManager*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Comment::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Comment::Class_Name()
{
   return "Comment";
}

//______________________________________________________________________________
const char *Comment::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Comment*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Comment::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Comment*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Comment::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Comment*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Comment::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Comment*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ExtensibleDb::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ExtensibleDb::Class_Name()
{
   return "ExtensibleDb";
}

//______________________________________________________________________________
const char *ExtensibleDb::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ExtensibleDb*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ExtensibleDb::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ExtensibleDb*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ExtensibleDb::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ExtensibleDb*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ExtensibleDb::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ExtensibleDb*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DataElement::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DataElement::Class_Name()
{
   return "DataElement";
}

//______________________________________________________________________________
const char *DataElement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataElement*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DataElement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DataElement*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DataElement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataElement*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DataElement::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DataElement*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ADCRegionInspector::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ADCRegionInspector::Class_Name()
{
   return "ADCRegionInspector";
}

//______________________________________________________________________________
const char *ADCRegionInspector::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADCRegionInspector*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ADCRegionInspector::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ADCRegionInspector*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ADCRegionInspector::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADCRegionInspector*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ADCRegionInspector::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ADCRegionInspector*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void ConfigEntry::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigEntry.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fNDets;
      R__b >> fNADCChannels;
      R__b >> fNTDCChannels;
      R__b >> isInitialized;
      R__b >> fRun;
      R__b >> fRunDuration;
      R__b >> fLiveTime;
      { TString R__str; R__str.Streamer(R__b); fTarget = R__str.Data(); }
      { TString R__str; R__str.Streamer(R__b); fPol = R__str.Data(); }
      { TString R__str; R__str.Streamer(R__b); fComment = R__str.Data(); }
      R__b >> fEnergy;
      delete [] fThresh;
      fThresh = new Float_t[fNADCChannels];
      R__b.ReadFastArray(fThresh,fNADCChannels);
      delete [] fPed;
      fPed = new Float_t[fNADCChannels];
      R__b.ReadFastArray(fPed,fNADCChannels);
      R__b >> fExtDb;
      {
         map<string,Int_t> &R__stl =  functionmap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            string R__t;
            {TString R__str;
             R__str.Streamer(R__b);
             R__t = R__str.Data();}
            int R__t2;
            R__b >> R__t2;
            typedef class std::basic_string<char> Value_t;
            std::pair<Value_t const, int > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b.CheckByteCount(R__s, R__c, ConfigEntry::IsA());
   } else {
      R__c = R__b.WriteVersion(ConfigEntry::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << fNDets;
      R__b << fNADCChannels;
      R__b << fNTDCChannels;
      R__b << isInitialized;
      R__b << fRun;
      R__b << fRunDuration;
      R__b << fLiveTime;
      { TString R__str = fTarget.c_str(); R__str.Streamer(R__b);}
      { TString R__str = fPol.c_str(); R__str.Streamer(R__b);}
      { TString R__str = fComment.c_str(); R__str.Streamer(R__b);}
      R__b << fEnergy;
      R__b.WriteFastArray(fThresh,fNADCChannels);
      R__b.WriteFastArray(fPed,fNADCChannels);
      R__b << fExtDb;
      {
         map<string,Int_t> &R__stl =  functionmap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<string,Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            {TString R__str(((*R__k).first ).c_str());
             R__str.Streamer(R__b);};
            R__b << ((*R__k).second);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigEntry(void *p) {
      return  p ? new(p) ::ConfigEntry : new ::ConfigEntry;
   }
   static void *newArray_ConfigEntry(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigEntry[nElements] : new ::ConfigEntry[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigEntry(void *p) {
      delete ((::ConfigEntry*)p);
   }
   static void deleteArray_ConfigEntry(void *p) {
      delete [] ((::ConfigEntry*)p);
   }
   static void destruct_ConfigEntry(void *p) {
      typedef ::ConfigEntry current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ConfigEntry(TBuffer &buf, void *obj) {
      ((::ConfigEntry*)obj)->::ConfigEntry::Streamer(buf);
   }
} // end of namespace ROOT for class ::ConfigEntry

//______________________________________________________________________________
void ConfigManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConfigManager.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b >> fNDetectors;
      {
         vector<ConfigEntry*> &R__stl =  fDb;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(class ConfigEntry *));
         if (R__tcl1==0) {
            Error("fDb streamer","Missing the TClass object for class ConfigEntry *!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            ConfigEntry* R__t;
            R__t = (ConfigEntry*)R__b.ReadObjectAny(R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Int_t> &R__stl =  listofruns;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      { TString R__str; R__str.Streamer(R__b); fFName = R__str.Data(); }
      { TString R__str; R__str.Streamer(R__b); fComment = R__str.Data(); }
      R__b >> fSpacer;
      R__b.CheckByteCount(R__s, R__c, ConfigManager::IsA());
   } else {
      R__c = R__b.WriteVersion(ConfigManager::IsA(), kTRUE);
      R__b << fNDetectors;
      {
         vector<ConfigEntry*> &R__stl =  fDb;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<ConfigEntry*>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Int_t> &R__stl =  listofruns;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      { TString R__str = fFName.c_str(); R__str.Streamer(R__b);}
      { TString R__str = fComment.c_str(); R__str.Streamer(R__b);}
      R__b << fSpacer;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConfigManager(void *p) {
      return  p ? new(p) ::ConfigManager : new ::ConfigManager;
   }
   static void *newArray_ConfigManager(Long_t nElements, void *p) {
      return p ? new(p) ::ConfigManager[nElements] : new ::ConfigManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConfigManager(void *p) {
      delete ((::ConfigManager*)p);
   }
   static void deleteArray_ConfigManager(void *p) {
      delete [] ((::ConfigManager*)p);
   }
   static void destruct_ConfigManager(void *p) {
      typedef ::ConfigManager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ConfigManager(TBuffer &buf, void *obj) {
      ((::ConfigManager*)obj)->::ConfigManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::ConfigManager

//______________________________________________________________________________
void ProofPlot::Streamer(TBuffer &R__b)
{
   // Stream an object of class ProofPlot.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ProofPlot(void *p) {
      delete ((::ProofPlot*)p);
   }
   static void deleteArray_ProofPlot(void *p) {
      delete [] ((::ProofPlot*)p);
   }
   static void destruct_ProofPlot(void *p) {
      typedef ::ProofPlot current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ProofPlot(TBuffer &buf, void *obj) {
      ((::ProofPlot*)obj)->::ProofPlot::Streamer(buf);
   }
} // end of namespace ROOT for class ::ProofPlot

//______________________________________________________________________________
void PP2d::Streamer(TBuffer &R__b)
{
   // Stream an object of class PP2d.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PP2d(void *p) {
      delete ((::PP2d*)p);
   }
   static void deleteArray_PP2d(void *p) {
      delete [] ((::PP2d*)p);
   }
   static void destruct_PP2d(void *p) {
      typedef ::PP2d current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PP2d(TBuffer &buf, void *obj) {
      ((::PP2d*)obj)->::PP2d::Streamer(buf);
   }
} // end of namespace ROOT for class ::PP2d

//______________________________________________________________________________
void PPCoincMap::Streamer(TBuffer &R__b)
{
   // Stream an object of class PPCoincMap.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PPCoincMap(void *p) {
      delete ((::PPCoincMap*)p);
   }
   static void deleteArray_PPCoincMap(void *p) {
      delete [] ((::PPCoincMap*)p);
   }
   static void destruct_PPCoincMap(void *p) {
      typedef ::PPCoincMap current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PPCoincMap(TBuffer &buf, void *obj) {
      ((::PPCoincMap*)obj)->::PPCoincMap::Streamer(buf);
   }
} // end of namespace ROOT for class ::PPCoincMap

//______________________________________________________________________________
void PPCoincMapRegions::Streamer(TBuffer &R__b)
{
   // Stream an object of class PPCoincMapRegions.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PPCoincMapRegions(void *p) {
      delete ((::PPCoincMapRegions*)p);
   }
   static void deleteArray_PPCoincMapRegions(void *p) {
      delete [] ((::PPCoincMapRegions*)p);
   }
   static void destruct_PPCoincMapRegions(void *p) {
      typedef ::PPCoincMapRegions current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PPCoincMapRegions(TBuffer &buf, void *obj) {
      ((::PPCoincMapRegions*)obj)->::PPCoincMapRegions::Streamer(buf);
   }
} // end of namespace ROOT for class ::PPCoincMapRegions

//______________________________________________________________________________
void SelectPPCoincMapRegions::Streamer(TBuffer &R__b)
{
   // Stream an object of class SelectPPCoincMapRegions.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SelectPPCoincMapRegions(void *p) {
      delete ((::SelectPPCoincMapRegions*)p);
   }
   static void deleteArray_SelectPPCoincMapRegions(void *p) {
      delete [] ((::SelectPPCoincMapRegions*)p);
   }
   static void destruct_SelectPPCoincMapRegions(void *p) {
      typedef ::SelectPPCoincMapRegions current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SelectPPCoincMapRegions(TBuffer &buf, void *obj) {
      ((::SelectPPCoincMapRegions*)obj)->::SelectPPCoincMapRegions::Streamer(buf);
   }
} // end of namespace ROOT for class ::SelectPPCoincMapRegions

//______________________________________________________________________________
void Visualizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class Visualizer.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Visualizer(void *p) {
      return  p ? new(p) ::Visualizer : new ::Visualizer;
   }
   static void *newArray_Visualizer(Long_t nElements, void *p) {
      return p ? new(p) ::Visualizer[nElements] : new ::Visualizer[nElements];
   }
   // Wrapper around operator delete
   static void delete_Visualizer(void *p) {
      delete ((::Visualizer*)p);
   }
   static void deleteArray_Visualizer(void *p) {
      delete [] ((::Visualizer*)p);
   }
   static void destruct_Visualizer(void *p) {
      typedef ::Visualizer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Visualizer(TBuffer &buf, void *obj) {
      ((::Visualizer*)obj)->::Visualizer::Streamer(buf);
   }
} // end of namespace ROOT for class ::Visualizer

//______________________________________________________________________________
void PFVisualizer::Streamer(TBuffer &R__b)
{
   // Stream an object of class PFVisualizer.

   Visualizer::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PFVisualizer(void *p) {
      delete ((::PFVisualizer*)p);
   }
   static void deleteArray_PFVisualizer(void *p) {
      delete [] ((::PFVisualizer*)p);
   }
   static void destruct_PFVisualizer(void *p) {
      typedef ::PFVisualizer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PFVisualizer(TBuffer &buf, void *obj) {
      ((::PFVisualizer*)obj)->::PFVisualizer::Streamer(buf);
   }
} // end of namespace ROOT for class ::PFVisualizer

//______________________________________________________________________________
void ScalerTool::Streamer(TBuffer &R__b)
{
   // Stream an object of class ScalerTool.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<Int_t> &R__stl =  sclrvals;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.StreamObject(&(scalerfile),typeid(scalerfile));
      R__b >> runloaded;
      R__b >> ch;
      R__b >> run;
      R__b.CheckByteCount(R__s, R__c, ScalerTool::IsA());
   } else {
      R__c = R__b.WriteVersion(ScalerTool::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<Int_t> &R__stl =  sclrvals;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<Int_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.StreamObject(&(scalerfile),typeid(scalerfile));
      R__b << runloaded;
      R__b << ch;
      R__b << run;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ScalerTool(void *p) {
      return  p ? new(p) ::ScalerTool : new ::ScalerTool;
   }
   static void *newArray_ScalerTool(Long_t nElements, void *p) {
      return p ? new(p) ::ScalerTool[nElements] : new ::ScalerTool[nElements];
   }
   // Wrapper around operator delete
   static void delete_ScalerTool(void *p) {
      delete ((::ScalerTool*)p);
   }
   static void deleteArray_ScalerTool(void *p) {
      delete [] ((::ScalerTool*)p);
   }
   static void destruct_ScalerTool(void *p) {
      typedef ::ScalerTool current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ScalerTool(TBuffer &buf, void *obj) {
      ((::ScalerTool*)obj)->::ScalerTool::Streamer(buf);
   }
} // end of namespace ROOT for class ::ScalerTool

//______________________________________________________________________________
void SScalerTool::Streamer(TBuffer &R__b)
{
   // Stream an object of class SScalerTool.

   ScalerTool::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SScalerTool(void *p) {
      return  p ? new(p) ::SScalerTool : new ::SScalerTool;
   }
   static void *newArray_SScalerTool(Long_t nElements, void *p) {
      return p ? new(p) ::SScalerTool[nElements] : new ::SScalerTool[nElements];
   }
   // Wrapper around operator delete
   static void delete_SScalerTool(void *p) {
      delete ((::SScalerTool*)p);
   }
   static void deleteArray_SScalerTool(void *p) {
      delete [] ((::SScalerTool*)p);
   }
   static void destruct_SScalerTool(void *p) {
      typedef ::SScalerTool current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SScalerTool(TBuffer &buf, void *obj) {
      ((::SScalerTool*)obj)->::SScalerTool::Streamer(buf);
   }
} // end of namespace ROOT for class ::SScalerTool

//______________________________________________________________________________
void FNameManager::Streamer(TBuffer &R__b)
{
   // Stream an object of class FNameManager.

   ::Error("FNameManager::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_FNameManager(void *p) {
      return  p ? new(p) ::FNameManager : new ::FNameManager;
   }
   static void *newArray_FNameManager(Long_t nElements, void *p) {
      return p ? new(p) ::FNameManager[nElements] : new ::FNameManager[nElements];
   }
   // Wrapper around operator delete
   static void delete_FNameManager(void *p) {
      delete ((::FNameManager*)p);
   }
   static void deleteArray_FNameManager(void *p) {
      delete [] ((::FNameManager*)p);
   }
   static void destruct_FNameManager(void *p) {
      typedef ::FNameManager current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_FNameManager(TBuffer &buf, void *obj) {
      ((::FNameManager*)obj)->::FNameManager::Streamer(buf);
   }
} // end of namespace ROOT for class ::FNameManager

//______________________________________________________________________________
void Comment::Streamer(TBuffer &R__b)
{
   // Stream an object of class Comment.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      R__b.CheckByteCount(R__s, R__c, Comment::IsA());
   } else {
      R__c = R__b.WriteVersion(Comment::IsA(), kTRUE);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Comment(void *p) {
      return  p ? new(p) ::Comment : new ::Comment;
   }
   static void *newArray_Comment(Long_t nElements, void *p) {
      return p ? new(p) ::Comment[nElements] : new ::Comment[nElements];
   }
   // Wrapper around operator delete
   static void delete_Comment(void *p) {
      delete ((::Comment*)p);
   }
   static void deleteArray_Comment(void *p) {
      delete [] ((::Comment*)p);
   }
   static void destruct_Comment(void *p) {
      typedef ::Comment current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Comment(TBuffer &buf, void *obj) {
      ((::Comment*)obj)->::Comment::Streamer(buf);
   }
} // end of namespace ROOT for class ::Comment

//______________________________________________________________________________
void ExtensibleDb::Streamer(TBuffer &R__b)
{
   // Stream an object of class ExtensibleDb.

   ::Error("ExtensibleDb::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ExtensibleDb(void *p) {
      return  p ? new(p) ::ExtensibleDb : new ::ExtensibleDb;
   }
   static void *newArray_ExtensibleDb(Long_t nElements, void *p) {
      return p ? new(p) ::ExtensibleDb[nElements] : new ::ExtensibleDb[nElements];
   }
   // Wrapper around operator delete
   static void delete_ExtensibleDb(void *p) {
      delete ((::ExtensibleDb*)p);
   }
   static void deleteArray_ExtensibleDb(void *p) {
      delete [] ((::ExtensibleDb*)p);
   }
   static void destruct_ExtensibleDb(void *p) {
      typedef ::ExtensibleDb current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ExtensibleDb(TBuffer &buf, void *obj) {
      ((::ExtensibleDb*)obj)->::ExtensibleDb::Streamer(buf);
   }
} // end of namespace ROOT for class ::ExtensibleDb

//______________________________________________________________________________
void DataElement::Streamer(TBuffer &R__b)
{
   // Stream an object of class DataElement.

   ::Error("DataElement::Streamer", "version id <=0 in ClassDef, dummy Streamer() called"); if (R__b.IsReading()) { }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DataElement(void *p) {
      delete ((::DataElement*)p);
   }
   static void deleteArray_DataElement(void *p) {
      delete [] ((::DataElement*)p);
   }
   static void destruct_DataElement(void *p) {
      typedef ::DataElement current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DataElement(TBuffer &buf, void *obj) {
      ((::DataElement*)obj)->::DataElement::Streamer(buf);
   }
} // end of namespace ROOT for class ::DataElement

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CombinationEngine(void *p) {
      delete ((::CombinationEngine*)p);
   }
   static void deleteArray_CombinationEngine(void *p) {
      delete [] ((::CombinationEngine*)p);
   }
   static void destruct_CombinationEngine(void *p) {
      typedef ::CombinationEngine current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CombinationEngine

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MultiplicityGroupGenerator(void *p) {
      delete ((::MultiplicityGroupGenerator*)p);
   }
   static void deleteArray_MultiplicityGroupGenerator(void *p) {
      delete [] ((::MultiplicityGroupGenerator*)p);
   }
   static void destruct_MultiplicityGroupGenerator(void *p) {
      typedef ::MultiplicityGroupGenerator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::MultiplicityGroupGenerator

namespace ROOT {
   // Wrapper around operator delete
   static void delete_FluxCalculator(void *p) {
      delete ((::FluxCalculator*)p);
   }
   static void deleteArray_FluxCalculator(void *p) {
      delete [] ((::FluxCalculator*)p);
   }
   static void destruct_FluxCalculator(void *p) {
      typedef ::FluxCalculator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FluxCalculator

//______________________________________________________________________________
void ADCRegionInspector::Streamer(TBuffer &R__b)
{
   // Stream an object of class ADCRegionInspector.

   TSelector::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ADCRegionInspector(void *p) {
      delete ((::ADCRegionInspector*)p);
   }
   static void deleteArray_ADCRegionInspector(void *p) {
      delete [] ((::ADCRegionInspector*)p);
   }
   static void destruct_ADCRegionInspector(void *p) {
      typedef ::ADCRegionInspector current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ADCRegionInspector(TBuffer &buf, void *obj) {
      ((::ADCRegionInspector*)obj)->::ADCRegionInspector::Streamer(buf);
   }
} // end of namespace ROOT for class ::ADCRegionInspector

namespace ROOT {
   // Wrappers around operator new
   static void *new_StripInfo(void *p) {
      return  p ? new(p) ::StripInfo : new ::StripInfo;
   }
   static void *newArray_StripInfo(Long_t nElements, void *p) {
      return p ? new(p) ::StripInfo[nElements] : new ::StripInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_StripInfo(void *p) {
      delete ((::StripInfo*)p);
   }
   static void deleteArray_StripInfo(void *p) {
      delete [] ((::StripInfo*)p);
   }
   static void destruct_StripInfo(void *p) {
      typedef ::StripInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StripInfo

namespace ROOT {
   // Wrapper around operator delete
   static void delete_StripDb(void *p) {
      delete ((::StripDb*)p);
   }
   static void deleteArray_StripDb(void *p) {
      delete [] ((::StripDb*)p);
   }
   static void destruct_StripDb(void *p) {
      typedef ::StripDb current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StripDb

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>),0);
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 210,
                  typeid(vector<int>), DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEConfigEntrymUgR_Dictionary();
   static void vectorlEConfigEntrymUgR_TClassManip(TClass*);
   static void *new_vectorlEConfigEntrymUgR(void *p = 0);
   static void *newArray_vectorlEConfigEntrymUgR(Long_t size, void *p);
   static void delete_vectorlEConfigEntrymUgR(void *p);
   static void deleteArray_vectorlEConfigEntrymUgR(void *p);
   static void destruct_vectorlEConfigEntrymUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ConfigEntry*>*)
   {
      vector<ConfigEntry*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ConfigEntry*>),0);
      static ::ROOT::TGenericClassInfo 
         instance("vector<ConfigEntry*>", -2, "vector", 210,
                  typeid(vector<ConfigEntry*>), DefineBehavior(ptr, ptr),
                  &vectorlEConfigEntrymUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<ConfigEntry*>) );
      instance.SetNew(&new_vectorlEConfigEntrymUgR);
      instance.SetNewArray(&newArray_vectorlEConfigEntrymUgR);
      instance.SetDelete(&delete_vectorlEConfigEntrymUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEConfigEntrymUgR);
      instance.SetDestructor(&destruct_vectorlEConfigEntrymUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ConfigEntry*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<ConfigEntry*>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEConfigEntrymUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ConfigEntry*>*)0x0)->GetClass();
      vectorlEConfigEntrymUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEConfigEntrymUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEConfigEntrymUgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<ConfigEntry*> : new vector<ConfigEntry*>;
   }
   static void *newArray_vectorlEConfigEntrymUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<ConfigEntry*>[nElements] : new vector<ConfigEntry*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEConfigEntrymUgR(void *p) {
      delete ((vector<ConfigEntry*>*)p);
   }
   static void deleteArray_vectorlEConfigEntrymUgR(void *p) {
      delete [] ((vector<ConfigEntry*>*)p);
   }
   static void destruct_vectorlEConfigEntrymUgR(void *p) {
      typedef vector<ConfigEntry*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ConfigEntry*>

namespace ROOT {
   static TClass *maplEstringcOintgR_Dictionary();
   static void maplEstringcOintgR_TClassManip(TClass*);
   static void *new_maplEstringcOintgR(void *p = 0);
   static void *newArray_maplEstringcOintgR(Long_t size, void *p);
   static void delete_maplEstringcOintgR(void *p);
   static void deleteArray_maplEstringcOintgR(void *p);
   static void destruct_maplEstringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,int>*)
   {
      map<string,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,int>),0);
      static ::ROOT::TGenericClassInfo 
         instance("map<string,int>", -2, "map", 96,
                  typeid(map<string,int>), DefineBehavior(ptr, ptr),
                  &maplEstringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,int>) );
      instance.SetNew(&new_maplEstringcOintgR);
      instance.SetNewArray(&newArray_maplEstringcOintgR);
      instance.SetDelete(&delete_maplEstringcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOintgR);
      instance.SetDestructor(&destruct_maplEstringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const map<string,int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,int>*)0x0)->GetClass();
      maplEstringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,int> : new map<string,int>;
   }
   static void *newArray_maplEstringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) map<string,int>[nElements] : new map<string,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOintgR(void *p) {
      delete ((map<string,int>*)p);
   }
   static void deleteArray_maplEstringcOintgR(void *p) {
      delete [] ((map<string,int>*)p);
   }
   static void destruct_maplEstringcOintgR(void *p) {
      typedef map<string,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,int>

namespace {
  void TriggerDictionaryInitialization_dictAnalyzer_Impl() {
    static const char* headers[] = {
"ConfigEntry.h",
"ConfigManager.h",
"ProofPlot.h",
"PP2d.h",
"PPCoincMap.h",
"PPCoincMapRegions.h",
"SelectPPCoincMapRegions.h",
"PAnalysis.h",
"Visualizer.h",
"PFVisualizer.h",
"ScalerTool.h",
"SScalerTool.h",
"Comment.h",
"FNameManager.h",
"ExtensibleDb.h",
"DataElement.h",
"Utils.h",
"CombinationEngine.h",
"MultiplicityGroupGenerator.h",
"FluxCalculator.h",
"ADCRegionInspector.h",
"StripInfo.h",
"UtilTemplates.h",
0
    };
    static const char* includePaths[] = {
"-I.",
"/home/jeromy/PhDPaper/analysis_soft/include",
"/usr/local/root/root-6.02.03/include",
"/home/jeromy/PhDPaper/analysis_soft/Analyzer/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$ConfigEntry.h")))  ConfigEntry;
class __attribute__((annotate("$clingAutoload$ConfigManager.h")))  ConfigManager;
class __attribute__((annotate("$clingAutoload$ProofPlot.h")))  ProofPlot;
class __attribute__((annotate("$clingAutoload$PP2d.h")))  PP2d;
class __attribute__((annotate("$clingAutoload$PPCoincMap.h")))  PPCoincMap;
class __attribute__((annotate("$clingAutoload$PPCoincMapRegions.h")))  PPCoincMapRegions;
class __attribute__((annotate("$clingAutoload$SelectPPCoincMapRegions.h")))  SelectPPCoincMapRegions;
class __attribute__((annotate("$clingAutoload$Visualizer.h")))  Visualizer;
class __attribute__((annotate("$clingAutoload$PFVisualizer.h")))  PFVisualizer;
class __attribute__((annotate("$clingAutoload$ScalerTool.h")))  ScalerTool;
class __attribute__((annotate("$clingAutoload$SScalerTool.h")))  SScalerTool;
class __attribute__((annotate("$clingAutoload$FNameManager.h")))  FNameManager;
class __attribute__((annotate("$clingAutoload$Comment.h")))  Comment;
class __attribute__((annotate("$clingAutoload$ConfigEntry.h")))  ExtensibleDb;
class __attribute__((annotate("$clingAutoload$ConfigEntry.h")))  DataElement;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$CombinationEngine.h")))  CombinationEngine;
class __attribute__((annotate("$clingAutoload$MultiplicityGroupGenerator.h")))  MultiplicityGroupGenerator;
class __attribute__((annotate("$clingAutoload$FluxCalculator.h")))  FluxCalculator;
class __attribute__((annotate("$clingAutoload$ADCRegionInspector.h")))  ADCRegionInspector;
class __attribute__((annotate("$clingAutoload$StripInfo.h")))  StripInfo;
class __attribute__((annotate("$clingAutoload$StripInfo.h")))  StripDb;
namespace std{template <class _T1, class _T2> struct __attribute__((annotate("$clingAutoload$string")))  pair;
}
typedef std::pair<StripInfo, StripInfo> StripPair __attribute__((annotate("$clingAutoload$StripInfo.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "ConfigEntry.h"
#include "ConfigManager.h"
#include "ProofPlot.h"
#include "PP2d.h"
#include "PPCoincMap.h"
#include "PPCoincMapRegions.h"
#include "SelectPPCoincMapRegions.h"
#include "PAnalysis.h"
#include "Visualizer.h"
#include "PFVisualizer.h"
#include "ScalerTool.h"
#include "SScalerTool.h"
#include "Comment.h"
#include "FNameManager.h"
#include "ExtensibleDb.h"
#include "DataElement.h"
#include "Utils.h"
#include "CombinationEngine.h"
#include "MultiplicityGroupGenerator.h"
#include "FluxCalculator.h"
#include "ADCRegionInspector.h"
#include "StripInfo.h"
#include "UtilTemplates.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ADCRegionInspector", payloadCode, "@",
"CombinationEngine", payloadCode, "@",
"Comment", payloadCode, "@",
"ConfigEntry", payloadCode, "@",
"ConfigManager", payloadCode, "@",
"DataElement", payloadCode, "@",
"ExtensibleDb", payloadCode, "@",
"FNameManager", payloadCode, "@",
"FluxCalculator", payloadCode, "@",
"MultiplicityGroupGenerator", payloadCode, "@",
"PFVisualizer", payloadCode, "@",
"PP2d", payloadCode, "@",
"PPCoincMap", payloadCode, "@",
"PPCoincMapRegions", payloadCode, "@",
"ProofPlot", payloadCode, "@",
"SScalerTool", payloadCode, "@",
"ScalerTool", payloadCode, "@",
"SelectPPCoincMapRegions", payloadCode, "@",
"StripDb", payloadCode, "@",
"StripInfo", payloadCode, "@",
"StripPair", payloadCode, "@",
"Visualizer", payloadCode, "@",
"vector<ConfigEntry*>", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictAnalyzer",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictAnalyzer_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictAnalyzer_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictAnalyzer() {
  TriggerDictionaryInitialization_dictAnalyzer_Impl();
}
