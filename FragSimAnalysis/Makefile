SIMNAME    := FragSim232Th
SIMDIR     := $(G4WORKDIR)/$(SIMNAME)
CMS         = $(HOME)/Code/Compilable
INSTALLDIR  = $(HOME)/Code/lib
G4TMPDIR   := $(G4WORKDIR)/tmp/$(G4SYSTEM)/$(SIMNAME) 

include $(CMS)/Common.mk

LIBNAME    := FragSimAnalyzer
SIMVERSION := 232Th
ROOTLIBS   := $(shell root-config --libs) -lProofPlayer
LIBS       += $(ROOTLIBS)
BINLIBS       += -L$(INSTALLDIR) -l$(LIBNAME)-$(SIMVERSION)
LIBS       += -L$(G4TMPDIR) -l$(SIMNAME)
INCLUDES   += -I$(SIMDIR)/include

#LIBS       += -L$(G4WORKDIR)/tmp/$(G4SYSTEM)/FragSim02 -lFragSim02

ifdef D
	CPPFLAGS += -D$(D)
endif

INCLUDES += $(shell pkg-config --cflags geant4)
INCLUDES += $(shell clhep-config --include)
INCLUDES += -I$(HOME)/Code/include
INCLUDES += -I$(SIMDIR)/include

LIBS += $(shell clhep-config --libs)
LIBS += -L/usr/lib -lboost_regex
LIBS += $(shell pkg-config  --libs geant4)
LIBS += -L~/PhDPaper/analysis_soft/lib -lFit
LIBS += -L$(HOME)/lib -lFragmentDists 
LIBS += -L$(HOME)/Code/lib -lROOTUtils -lNuclearChart


CPPFLAGS += $(INCLUDES)

libheaders := TSelectorInsert.h SiStripGenerator.h AngleAverager.h SolidAngleComputer.h EnergyHist.h TTreeProxy.h Analyzer.h LambdaGenerator.h EtaDbFiller2C.h SolidAnglePDFGeneratorC.h YieldGenerator.h RatioVsOrigin.h EnergyHistSorted.h LinkDef.h
libobjs    := SiStripGenerator.o AngleAverager.o SolidAngleComputer.o EnergyHist.o TTreeProxy.o Analyzer.o LambdaGenerator.o EtaDbFiller2C.o SolidAnglePDFGeneratorC.o YieldGenerator.o RatioVsOrigin.o EnergyHistSorted.o dict$(LIBNAME).o


INSTALLHEADERS = $(filter-out LinkDef.h, $(libheaders)) 

.PHONEY : all install clean

%.o : %.cpp 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ -c $^ 

#test : test.o
#	rm -f $@
#	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS)

#integrate_marg_eta2 : integrate_marg_eta2.o
#	rm -f $@
#	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS) $(BINLIBS)

analyze : analyze.o
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS) $(BINLIBS)

analyze_lambda : analyze_lambda.o
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS) $(BINLIBS)

analyze_all : analyze_all.o
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS) $(BINLIBS)


combine : combine.o $(BINLIBS)
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS)

test_boundary_finder : test_boundary_finder.o
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS)

all : lib$(LIBNAME)-$(SIMVERSION).so

lib$(LIBNAME)-$(SIMVERSION).so : $(libobjs)
	$(CPP) -Wl -shared -o $@ $^ $(LIBS)

dict$(LIBNAME).cpp : $(libheaders)
	$(ROOTSYS)/bin/rootcint -f $@ -c -I$(INCLUDES) $(libheaders)

install :
	@echo "Installing lib$(LIBNAME)-$(SIMVERSION).so --> $(INSTALLDIR)/lib$(LIBNAME)-$(SIMVERSION).so"
	@cp lib$(LIBNAME)-$(SIMVERSION).so $(INSTALLDIR)/lib$(LIBNAME)-$(SIMVERSION).so
	@echo "Copying headers into ${INSTALLDIR}/../include" 
	@cp ${INSTALLHEADERS} ${INSTALLDIR}/../include/.   

clean :
	rm -f dict*
	rm -f *.o


