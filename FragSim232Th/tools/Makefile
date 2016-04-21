CMS         = /home/jrtomps/Code/Compilable
INSTALLDIR  = /home/jrtomps/Code/lib

include $(CMS)/Common.mk

ifdef CLHEP_INCLUDE_DIR
	CPPFLAGS += -I$(CLHEP_INCLUDE_DIR)
else
	@echo "Warning CLHEP_INCLUDE_DIR is undefined!"
endif

ifdef CLHEP_LIB_DIR
	LIBS += -L$(CLHEP_LIB_DIR) -lCLHEP
else
	@echo "Warning CLHEP_LIB_DIR is undefined!"
	
endif

CPPFLAGS += -I$(G4INSTALL)/source/global/management/include
CPPFLAGS += -I$(G4INSTALL)/source/global/HEPGeometry/include


G4LIBS  := -L$(G4INSTALL)/lib/$(G4SYSTEM) \
	-lG4interfaces -lG4run -lG4digits_hits -lG4materials \
	-lG4tracking -lG4error_propagation -lG4modeling -lG4track \
 	-lG4event -lG4OpenGL -lG4Tree -lG4FR -lG4parmodels \
	-lG4visHepRep -lG4geometry -lG4particles -lG4vis_management \
	-lG4gl2ps -lG4persistency -lG4visXXX -lG4global \
	-lG4physicslists -lG4VRML -lG4GMocren -lG4processes \
	-lG4graphics_reps -lG4RayTracer -lG4intercoms -lG4readout

PROJNAME := FragSim232Th
LIBS    += -L$(G4WORKDIR)/tmp/$(G4SYSTEM)/$(PROJNAME) -l$(PROJNAME)
LIBS    += $(G4LIBS)
LIBS    += $(shell root-config --libs)

LIBS    += -L/home/jrtomps/pffrag2/analysis/analysis_soft/lib -lAnalyzer -lFit

CPPFLAGS += -I$(G4WORKDIR)/$(PROJNAME)/include

ifdef D
	CPPFLAGS += -D$(D)
endif

%.o : %.cpp 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ -c $^ 

TargetOffsetCalculator232th : TargetOffsetCalculator232th.o 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIBS)

TargetOffsetCalculator : TargetOffsetCalculator.o 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIBS)

compute_target_positions2 : compute_target_positions2.o 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIBS)

compute_target_positions : compute_target_positions.o 
	rm -f $@
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIBS)

clean :
	rm -f *.o


