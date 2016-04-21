
MAKE    = make
CPP	= clang++ -std=c++11
CXX = clang++ -std=c++11
CC	= clang
FF	= gfortran

DEPENDS_FLAGS += -Wno-deprecated

MYLIBDIR = $(Analysis_Top_Dir)/lib
MYINCDIR = $(Analysis_Top_Dir)/include
MYSRCDIR = .

INCLUDES += -I. -I${MYINCDIR}
CFLAGS	 += -O2 $(INCLUDES)
CPPFLAGS += -O2 $(INCLUDES)

CSRC    = $(wildcard *.c)
CPPSRC	= $(wildcard *.cpp)

COBJS	= $(CSRC:.c=.o)
CPPOBJS	= $(CPPSRC:.cpp=.o)
ROOT_DICT_OBJS = $(CPPSRC:.cpp=Dict.o)

OBJS =  $(COBJS) $(CPPOBJS) $(ROOT_DICT_OBJS)

DEPS0 = $(COBJS) $(CPPOBJS) $(ROOT_DICT_OBJS)
DEPS1 = $(DEPS0:.o=$(DEBUG_SUFFIX).d)
DEPS  = $(foreach dfile,$(DEPS1),depends/$(dfile))


ifdef DEBUG
  DEBUG_SUFFIX = _debug
  CFLAGS      += -g -pg -DDEBUG
  CPPFLAGS    += -g -pg -DDEBUG
endif

ifdef D
  CFLAGS      += -D$(D)
  CPPFLAGS    += -D$(D)
endif

ifdef ROOTSYS
  ROOTCFLAGS   = $(shell root-config --cflags)
  ROOTGLIBS    = $(shell root-config --glibs) -lXMLIO

  CFLAGS      += $(ROOTCFLAGS) 
  CPPFLAGS    += $(ROOTCFLAGS) -fPIC
  LD_FLAGS    += $(ROOTGLIBS)
endif

MODULE_NAME = $(shell basename $(shell pwd))

LIBNAME = $(MYLIBDIR)/lib$(MODULE_NAME)$(DEBUG_SUFFIX).a

SLIBNAME = $(MYLIBDIR)/lib$(MODULE_NAME)$(DEBUG_SUFFIX).so

ifeq ($(OS),Linux)
  CPPFLAGS += -Wno-deprecated
endif

ifeq ($(OS), Darwin)
  CPPFLAGS += -I/usr/X11R6/include
  MISC_LIBS += -L/usr/X11R6/lib
endif

#CODEDIR = $(shell echo ${CODEDIR})
