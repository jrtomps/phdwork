-include $(Analysis_Top_Dir)/Common.mk

LIBNAME := Fit
 
LIBS := $(shell root-config --glibs) -lMathMore -lFoam \
		$(shell clhep-config --libs) \
		-L$(CODEDIR)/lib -lROOTUtils


#G4SIMNAME := FragSim232Th
#G4ANALDIR := /home/jeromy/Exps/G4Work/$(G4SIMNAME)/analysis
#G4INCLUDES += -I$(G4ANALDIR)
#CPPFLAGS += $(G4INCLUDES)
#INCLUDES += $(G4INCLUDES)

INCLUDES +=  $(shell clhep-config --include)
INCLUDES += -I$(Analysis_Top_Dir)/SystematicFitter -I$(CODEDIR)/include

CPPFLAGS += $(INCLUDES)

G4LIBS += -L$(CODEDIR)/lib -lFragmentDists
LIBS += $(G4LIBS) 

DONTMAKEOBJS := ConverterMultiDim.h Polarity.h 

libheaders := $(filter-out $(DONTMAKEOBJS) LinkDef.h dict$(LIBNAME).h, $(wildcard *.h))

targets := $(patsubst %.h,%,$(libheaders))

libobjs += $(addsuffix .o,$(targets)) dict$(LIBNAME).o

rootcintheaders := $(libheaders)
//rootcintheaders := $(filter-out MDataRange.h, $(libheaders))
rootcintheaders += LinkDef.h

.PHONEY : all clean printenv install

all : lib$(LIBNAME).so 

chisq_main_0 : test_template.o 
	$(CPP) $(CPPFLAGS) -g -o $@ $< $(LIBS)

printenv :
	@echo "LIBS=$(LIBS)"
	@echo "CPPFLAGS=$(CPPFLAGS)"
	@echo "CODEDIR=$(CODEDIR)"
	@echo $(ROOTSYS)
	@echo $(targets)
	@echo $(libheaders)
	@echo $(libobjs)

install :
	@echo "Copying lib$(LIBNAME).so --> $(Analysis_Top_Dir)/lib"
	@cp lib$(LIBNAME).so $(Analysis_Top_Dir)/lib/.
	@echo "Copying lib$(LIBNAME).so --> $(CODEDIR)/lib"
	@cp lib$(LIBNAME).so $(CODEDIR)/lib/.
	@echo "Copying headers --> $(Analysis_Top_Dir)/include $(CODEDIR)/include"
	@for name in ${libheaders} ${DONTMAKEOBJS}; do                        \
		if [ $$name != "LinkDef.h" ] ; then	  \
			cp $$name ${Analysis_Top_Dir}/include/. ;\
			cp $$name $(CODEDIR)/include/. ;\
		fi \
	done

$(templates).o : $(templates).h
	$(CPP) $(CPPFLAGS) -c -g -o $@ $< 


lib$(LIBNAME).so : $(libobjs)
	$(CPP) -Wl,-rpath=$(ROOTSYS)/lib -shared -o $@ $^ $(LIBS)

dict$(LIBNAME).cpp : $(rootcintheaders)
	$(ROOTSYS)/bin/rootcint -f $@ -c $(INCLUDES) $(rootcintheaders)

clean :
	rm -f *.o
	rm -f dict*
