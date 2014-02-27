ROOTCFLAGS    = $(shell root-config --cflags)
ROOTGLIBS     = $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -fPIC -ansi -D_GNU_SOURCE -O2 -Wall -Wextra 
LDFLAGS       = -g3 

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

Retina: Retina.o LayerGeometry.o HitCollection.o HitsGenerator.o ConformalTransf.o RetinaTrackFitter.o CommonFuncs.o
	$(CXX) -o $@ $^  $(LDFLAGS) $(GLIBS)

RetinaSimple: RetinaSimple.o LayerGeometry.o HitCollection.o HitsGenerator.o ConformalTransf.o RetinaTrackFitter.o CommonFuncs.o
	$(CXX) -o $@ $^  $(LDFLAGS) $(GLIBS)

Retina.o: Retina.cc print.h
	$(CXX) $(CXXFLAGS) -c Retina.cc -o $@

RetinaSimple.o: RetinaSimple.cc print.h
	$(CXX) $(CXXFLAGS) -c RetinaSimple.cc -o $@

RetinaTrackFitter.o: RetinaTrackFitter.cpp RetinaTrackFitter.h
	$(CXX) $(CXXFLAGS) -c RetinaTrackFitter.cpp  -o $@

ConformalTransf.o: ConformalTransf.cpp ConformalTransf.h
	$(CXX) $(CXXFLAGS) -c ConformalTransf.cpp -o $@

HitsGenerator.o: HitsGenerator.cpp HitsGenerator.h
	$(CXX) $(CXXFLAGS) -c HitsGenerator.cpp -o $@

HitCollection.o: HitCollection.cpp HitCollection.h
	$(CXX) $(CXXFLAGS) -c HitCollection.cpp -o $@

LayerGeometry.o: LayerGeometry.cpp LayerGeometry.h
	$(CXX) $(CXXFLAGS) -c LayerGeometry.cpp -o $@

CommonFuncs.o: CommonFuncs.cpp CommonFuncs.h
	$(CXX) $(CXXFLAGS) -c CommonFuncs.cpp -o $@

clean:
	rm -f Retina RetinaSimple Retina.o RetinaSimple.o LayerGeometry.o HitCollection.o HitsGenerator.o ConformalTransf.o RetinaTrackFitter.o CommonFuncs.o


