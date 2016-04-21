-include $(Analysis_Top_Dir)/Common.mk

LIBS := $(shell root-config --glibs) libAnalyzer.so
LIBS += -L$(CODEDIR)/lib -lROOTUtils

CPPFLAGS += -I$(CODEDIR)/include

libobjs := ConfigEntry.o ConfigManager.o ProofPlot.o PP2d.o \
			PPCoincMap.o PPCoincMapRegions.o SelectPPCoincMapRegions.o \
           PAnalysis.o \
           Visualizer.o PFVisualizer.o ScalerTool.o \
           SScalerTool.o Comment.o FNameManager.o ExtensibleDb.o \
           DataElement.o Utils.o CombinationEngine.o \
			MultiplicityGroupGenerator.o FluxCalculator.o \
			ADCRegionInspector.o  StripInfo.o dictAnalyzer.o

libheaders := ConfigEntry.h ConfigManager.h ProofPlot.h PP2d.h PPCoincMap.h \
			  PPCoincMapRegions.h SelectPPCoincMapRegions.h PAnalysis.h \
              Visualizer.h PFVisualizer.h \
              ScalerTool.h SScalerTool.h Comment.h FNameManager.h \
              ExtensibleDb.h DataElement.h Utils.h CombinationEngine.h \
			  MultiplicityGroupGenerator.h FluxCalculator.h \
				ADCRegionInspector.h StripInfo.h UtilTemplates.h LinkDef.h

.PHONEY : all clean printenv

all : libAnalyzer.so 

analyzerun : analyzerun.o 
	$(CPP) $(CPPFLAGS) -g -o $@ $< $(LIBS)

test : test.o 
	$(CPP) $(CPPFLAGS) -g -o $@ $< $(LIBS)

printenv :
	@echo "LIBS=$(LIBS)"
	@echo "CPPFLAGS=$(CPPFLAGS)"

libAnalyzer.so : $(libobjs)
	$(CPP) -Wl,-soname=$@ -shared -o $@ $^ $(filter-out libAnalyzer.so, $(LIBS))

dictAnalyzer.cpp : $(libheaders)
	$(ROOTSYS)/bin/rootcint -f $@ -c -I$(INCLUDES) $(libheaders)

clean :
	rm -f *.o

install :
	@echo "Copying libAnalyzer.so --> $(Analysis_Top_Dir)/lib"
	@cp libAnalyzer.so $(Analysis_Top_Dir)/lib/.
	@echo "Copying headers --> $(Analysis_Top_Dir)/include"
	@for name in $(shell ls *.h) ; do                        \
		if [ $$name != "LinkDef.h" ] ; then	  \
			if [ $$name != "dictAnalyzer.h" ] ; then    \
			cp $$name ${Analysis_Top_Dir}/include/. ;\
			fi \
		fi \
	done
