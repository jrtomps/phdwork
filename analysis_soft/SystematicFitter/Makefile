-include $(Analysis_Top_Dir)/Common.mk

LIBNAME := SystematicFitter
 
LIBS := $(shell root-config --glibs) \
		$(shell clhep-config --libs)

CPPFLAGS += $(shell clhep-config --include)

blacklist := FitParameterGenerator 

libheaders := $(filter-out LinkDef.h dict$(LIBNAME).h, $(wildcard *.h))

all_targets := $(patsubst %.h,%,$(libheaders))

targets := $(filter-out $(blacklist), $(all_targets))

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
	@echo $(ROOTSYS)
	@echo $(targets)
	@echo $(libheaders)
	@echo $(libobjs)

install :
	@echo "Copying lib$(LIBNAME).so --> $(Analysis_Top_Dir)/lib"
	@cp lib$(LIBNAME).so $(Analysis_Top_Dir)/lib/.
	@echo "Copying headers --> $(Analysis_Top_Dir)/include"
	@for name in ${libheaders} ; do                        \
		if [ $$name != "LinkDef.h" ] ; then	  \
			cp $$name ${Analysis_Top_Dir}/include/. ;\
		fi \
	done

$(templates).o : $(templates).h
	$(CPP) $(CPPFLAGS) -c -g -o $@ $< 


lib$(LIBNAME).so : $(libobjs)
	$(CPP) -Wl -shared -o $@ $^ $(LIBS)

dict$(LIBNAME).cpp : $(rootcintheaders)
	$(ROOTSYS)/bin/rootcint -f $@ -c $(INCLUDES) $(rootcintheaders)

clean :
	rm -f *.o
	rm -f dict*
