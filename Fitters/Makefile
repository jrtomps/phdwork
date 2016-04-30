-include $(Analysis_Top_Dir)/Common.mk

LIBS += $(shell root-config --glibs) -lMathMore -lMinuit2
LIBS +=	-L$(Analysis_Top_Dir)/lib -lFit -lSystematicFitter
#LIBS += -L/home/jrtomps/Code/lib -lsplitstream
LIBS += $(shell clhep-config --ldflags)

G4SIMNAME := FragSim232Th
G4ANALDIR := /home/jeromy/Exps/G4Work/$(G4SIMNAME)/analysis

G4INCLUDES += -I$(G4ANALDIR)
CPPFLAGS += $(G4INCLUDES)
INCLUDES += $(G4INCLUDES)
INCLUDES += $(shell clhep-config --include)

G4LIBS += $(shell pkg-config --libs geant4)
G4LIBS += -L/home/jeromy/Exps/G4Work/tmp/Linux-g++/FragSim232Th -lFragSim232Th
G4LIBS += -L/usr/lib -lboost_regex
LIBS += $(G4LIBS) 

MYINCLUDES += -I/home/jeromy/Code/include
MYINCLUDES += -I$(Analysis_Top_Dir)/SystematicFitter
MYLIBS += -L$(CODEDIR)/lib -lROOTUtils -lFragmentDists

CPPFLAGS += $(MYINCLUDES)
LIBS += $(MYLIBS)

all_source := $(wildcard *.cpp)

blacklist := CreateTF2FromAngDist VisualizeFCN_ChSq CreateRandomPlane \
			VisualizeMultiDimSurface test_paramfunctor

targets := $(filter-out $(blacklist),$(patsubst %.cpp,%,$(all_source)))

.PHONEY : all clean printenv install

all : $(targets)

$(targets) : % : %.o	
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS)

printenv :
	@echo "LIBS=$(LIBS)"
	@echo "CPPFLAGS=$(CPPFLAGS)"
	@echo $(ROOTSYS)
	@echo $(targets)

install :
	@echo "Copying $(targets) --> ${Analysis_Top_Dir}/bin"
	@for name in ${targets} ; do                        \
		cp $$name ${Analysis_Top_Dir}/bin/. ;\
	done

clean :
	rm -f *.o
	rm -f dict*
