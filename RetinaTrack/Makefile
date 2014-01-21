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
#BOOST		= -I/usr/local/boost_1_55_0
CXX	         += -I$(INCLUDEDIR) $(BOOST)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

all: Retina 

Retina: Retina.cc HitsGenerator LayerGeometry RetinaTrackFitter CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -o Retina.o LayerGeometry.o HitsGenerator.o RetinaTrackFitter.o CommonFuncs.cpp $(GLIBS)  $(LDFLAGS)  $<

RetinaSimple: RetinaSimple.cc LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -o RetinaSimple.o LayerGeometry.o HitCollection.o HitsGenerator.o ConformalTransf.o RetinaTrackFitter.o CommonFuncs.cpp $(GLIBS)  $(LDFLAGS)  $<

RetinaTrackFitter: RetinaTrackFitter.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c RetinaTrackFitter.cpp -o RetinaTrackFitter.o

ConformalTransf: ConformalTransf.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c ConformalTransf.cpp -o ConformalTransf.o

HitsGenerator: HitsGenerator.cpp CommonFuncs.cpp 
		$(CXX) $(CXXFLAGS) -c HitsGenerator.cpp -o HitsGenerator.o 

HitCollection: HitCollection.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c HitCollection.cpp -o HitCollection.o

LayerGeometry: LayerGeometry.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c LayerGeometry.cpp -o LayerGeometry.o

#CommonFuncs: CommonFuncs.cpp
#		$(CXX) $(CXXFLAGS) -o CommonFuncs.o $(GLIBS)  $(LDFLAGS)  $<

clean:
	rm -f *.o *~


