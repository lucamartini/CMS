ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -fPIC -ansi -D_GNU_SOURCE -O2 -Wall -Wextra 
LDFLAGS       = -g3 -lMinuit -lFoam -lGraf -lPostscript

SOFLAGS       = -shared

ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)

CXXFLAGS      += $(ROOTCFLAGS)
NGLIBS         = $(ROOTGLIBS) 
GLIBS          = $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR       = ./
CXX	         += -I$(INCLUDEDIR)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

all: Retina RetinaSimple

Retina: Retina.cc LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -o Retina LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter CommonFuncs.cpp $(GLIBS)  $(LDFLAGS)  $<

RetinaSimple: RetinaSimple.cc LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -o RetinaSimple LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter CommonFuncs.cpp $(GLIBS)  $(LDFLAGS)  $<

RetinaTrackFitter: RetinaTrackFitter.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c RetinaTrackFitter.cpp -o RetinaTrackFitter

ConformalTransf: ConformalTransf.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c ConformalTransf.cpp -o ConformalTransf

HitsGenerator: HitsGenerator.cpp CommonFuncs.cpp 
		$(CXX) $(CXXFLAGS) -c HitsGenerator.cpp -o HitsGenerator 

HitCollection: HitCollection.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c HitCollection.cpp -o HitCollection

LayerGeometry: LayerGeometry.cpp CommonFuncs.cpp
		$(CXX) $(CXXFLAGS) -c LayerGeometry.cpp -o LayerGeometry

clean:
	rm Retina RetinaSimple LayerGeometry HitCollection HitsGenerator ConformalTransf RetinaTrackFitter


