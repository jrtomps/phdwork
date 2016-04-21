
-include $(CODEDIR)/Compilable/Common.mk

LIBNAME     = ROOTUtils
INSTALLDIR  = $(CODEDIR)/lib

libobjs := STDatime.o HistFiller.o ROOTUtils.o VectorSorter.o VectorWriter.o \
			ColumnFileLoader.o DataBucket.o TreeMaker.o SingleTreeMaker.o MultiTreeMaker.o \
			ConsistentAxisConverter.o TH2GaussianSmear.o TH2CosPhiSmear.o dictROOTUtils.o 

libheaders := $(filter-out dictROOTUtils.h, $(wildcard *.h))
libheaders := $(filter-out LinkDef.h, $(libheaders))
libheaders += LinkDef.h

CPPFLAGS += -I/usr/include/boost
LIBS     += -L/usr/lib -lboost_regex
LIBS     += $(shell root-config --glibs) -Wl,-rpath=${ROOTSYS}/lib

.PHONEY : build install clean

build : lib$(LIBNAME).so 

install :
	@echo "Installing lib$(LIBNAME).so --> $(INSTALLDIR)/lib$(LIBNAME).so"
	@cp lib$(LIBNAME).so $(INSTALLDIR)/lib$(LIBNAME).so
	@echo "Copying headers --> $(CODEDIR)/include"
	@cp $(libheaders) $(CODEDIR)/include


%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

lib$(LIBNAME).so : $(libobjs)
	$(CPP) -Wl -shared -o $@ $^ $(LIBS)

dict$(LIBNAME).cpp : $(libheaders)
	$(ROOTSYS)/bin/rootcint -f $@ -c -I/usr/include/boost $(INCLUDES) $(libheaders)

clean : 
	rm -f *.o
	rm -f dict*
