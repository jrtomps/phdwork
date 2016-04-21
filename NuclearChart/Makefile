
ifndef MYCODEDIR
COMPILABLEDIR = $(HOME)/Code/Compilable
endif

-include ../Common.mk

MAJOR := NuclearChart

#GSLDIR := /usr/local/gsl-1.9

LIBS := $(shell root-config --glibs) -Wl,-rpath=${ROOTSYS}/lib

libobjs := $(MAJOR).o Isotope.o dict$(MAJOR).o

libheaders := $(MAJOR).h Isotope.h 

.PHONEY : build install all clean delete

all : build install  

build : lib$(MAJOR).so

install :
	@echo "Installing lib$(MAJOR).so --> $(CODEDIR)/lib"
	@cp lib$(MAJOR).so $(CODEDIR)/lib/.
	@echo "Copying headers --> $(CODEDIR)/include"
	@cp ${libheaders} $(CODEDIR)/include/.

test : test.o 
	$(CPP) $(CPPFLAGS) -g -o $@ $< -L. -lNuclearChart $(LIBS) -L/usr/lib -lboost_regex

main : main.o
	$(CPP) $(CPPFLAGS) -o $@ $< $(LIBS) -l$(MAJOR)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

lib$(MAJOR).so : $(libobjs)
	$(CPP) -Wl -shared -o $@ $^ $(LIBS)

dict$(MAJOR).cpp : $(libheaders) LinkDef.h
	$(ROOTSYS)/bin/rootcint -f $@ -c -p -I$(INCLUDES) $(libheaders)

clean : 
	rm -f *.o

delete:
	rm -f lib$(MAJOR).so
	rm -f dict*
	rm -f *.o

