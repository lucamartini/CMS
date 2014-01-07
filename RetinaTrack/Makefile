ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)
#ROOTCFLAGS    = -pthread -m64 -I/Users/lucamartini/root/include
#ROOTGLIBS     = -L/Users/lucamartini/root/lib -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lpthread -Wl,-rpath,/Users/lucamartini/root/lib -lm -ldl

CXX           = g++
#CXX		=/usr/local/bin/g++
CXXFLAGS      = -fPIC -ansi -D_GNU_SOURCE -O2 -Wall -Wextra 
LDFLAGS       = -g3 -lRooFit -lRooFitCore -lMinuit -lFoam -lGraf -lRooStats -lTMVA -lPostscript

SOFLAGS       = -shared

ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)
#ARCH         := macosx64
#PLATFORM     := macosx

CXXFLAGS      += $(ROOTCFLAGS)
NGLIBS         = $(ROOTGLIBS) 
GLIBS          = $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR       = ./
BOOST		= -I/usr/local/boost_1_55_0
CXX	         += -I$(INCLUDEDIR)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

all: makeRetina

makeRetina: makeRetina.cc RetinaTrackFitter
		$(CXX) $(CXXFLAGS) $(BOOST) -o makeRetina.o  RetinaTrackFitter.o $(GLIBS)  $(LDFLAGS)  $<

RetinaTrackFitter: RetinaTrackFitter.cpp
		$(CXX) $(CXXFLAGS) $(BOOST) -c RetinaTrackFitter.cpp

clean:
	rm -f *.o *~


