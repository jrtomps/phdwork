# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := FragSim232Th
G4TARGET := $(name)
G4EXLIB := true


ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all : lib bin

include $(G4INSTALL)/config/binmake.gmk

CPPFLAGS += -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui
pffrag2_soft_dir=/home/jeromy/PhDPaper/analysis_soft
CPPFLAGS += -I$(pffrag2_soft_dir)/include

MYLIBS += -L$(CODEDIR)/lib -lROOTUtils -lFit -lNuclearChart
MYINCLUDES += -I$(CODEDIR)/include

LDLIBS += $(MYLIBS)
CPPFLAGS += $(MYINCLUDES)



ifdef DEBUG
	CPPFLAGS  += -g
endif

ifdef D
	CPPFLAGS  += -D$(D)
endif


########################### ROOT #################################
# Copied from:
# GNUmakefile for IcaG4.  Jacek M. Holeczek Fri May 10 2002.
# Reference: http://root.cern.ch/root/HowtoMC.html

ifdef ROOTSYS
  ifndef G4UI_USE_ROOT
    ROOTCPPFLAGS   = $(shell $(ROOTSYS)/bin/root-config --cflags) # for compiling on riesling
    CPPFLAGS      += -DG4ANALYSIS_USE_ROOT $(ROOTCPPFLAGS)
    CPPFLAGS+=-Wno-deprecated
    ROOTLIBS       = $(shell $(ROOTSYS)/bin/root-config --nonew --glibs)
    ROOTLIBS      := $(filter-out -lNew,$(ROOTLIBS))
    ROOTLIBS      += -lMinuit
    LDLIBS        += $(ROOTLIBS)
  endif
endif

#TMPDIR := $(G4WORKDIR)/tmp/$(G4SYSTEM)/$(G4TARGET)
#buildroot : $(TMPDIR)/libDataBucket.so

#$(TMPDIR)/libDataBucket.so : $(TMPDIR)/dictDataBucket.o $(TMPDIR)/DataBucket.o
#	@echo "Linking libDataBucket.so"
#	@g++ $(LDFLAGS) -shared -Wl,-soname=$@ -o $@ $^ $(ROOTLIBS)
#	@export LDLIBS="$(LDLIBS) -L$(TMPDIR) -lDataBucket"
#	@cp $@ $(CODEDIR)/lib/libDataBucket.so

#$(TMPDIR)/dictDataBucket.o : $(G4WORKDIR)/$(name)/src/dictDataBucket.cc
#	@g++ $(CPPFLAGS) -I$(G4WORKDIR)/$(name)/include -c -o $@ $<

#$(G4WORKDIR)/$(name)/src/dictDataBucket.cc : $(G4WORKDIR)/$(name)/include/DataBucket.hh $(G4WORKDIR)/$(name)/include/LinkDef.h 
#	@$(ROOTSYS)/bin/rootcint -f $@ -c -p $^

printenv :
	@printenv | grep G4
	@if [ "$(G4UI_USE_QT)x" != "x" ] ; then \
		echo "G4UI_USE_QT=$(G4UI_USE_QT)"; \
	fi 
	@echo "CPPFLAGS = $(CPPFLAGS) " 
	@echo "LDLIBS   = $(LDLIBS) "
