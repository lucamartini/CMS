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
GLIBS          = $(filter-out -lNew, $(NGLIBS)) -lTreePlayer

INCLUDEDIR       = ./
CXX	         += -I$(INCLUDEDIR)

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ../../../../../lib/

###########
# TARGETS #
###########

charge: charge.cc 
		$(CXX) $(CXXFLAGS) -o charge $(GLIBS)  $(LDFLAGS)  $<

DeltaR: DeltaR.cc 
		$(CXX) $(CXXFLAGS) -o DeltaR $(GLIBS)  $(LDFLAGS)  $<

clean:
	rm -rf charge DeltaR


