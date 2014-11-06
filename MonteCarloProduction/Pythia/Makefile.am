AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(includedir) -I$(OFFLINE_MAIN)/include -I`root-config --incdir` -I$(PYTHIA8)/include -I$(HEPMC)/include -I$(LHAPDF)/include -I/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/HepMC/install/include
AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib -L$(OPT_PHENIX)/lib -L$(PYTHIA8)/lib -L$(HEPMC)/lib -L/usr/lib -L$(LHAPDF)/lib -L/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/HepMC/install/lib

AM_CXXFLAGS = -Wall -Werror

lib_LTLIBRARIES = libPHPythia.la libPHPythiaEventGen.la

include_HEADERS = \
  PHPythiaContainer.h \
  PHPythiaContainerV2.h \
  PHPythiaHeader.h \
  PHPythiaHeaderV1.h \
  PHPythiaHeaderV2.h \
  PHPyOscarOutputManager.h \
  PHPyVertexShift.h \
  PHPythiaAncestryHelper.h \
  PHPythia.h \
  PHPythia8.h \
  PHPy8GenTrigger.h \
  PHPy8ParticleTrigger.h \
  PHPyCommon.h

libPHPythia_la_LDFLAGS = ${AM_LDFLAGS} `root-config --libs`
libPHPythia_la_LIBADD = \
  -lphool -lSubsysReco -lfun4all -lPythia6 -lEG -lEGPythia6 -lz -lboost_iostreams -lpythia8 -lpythia8tohepmc -lHepMC -lLHAPDF 

libPHPythia_la_SOURCES = \
  PHPythiaHeader.C \
  PHPythiaHeaderV1.C \
  PHPythiaHeaderV2.C \
  PHPythiaContainer.C \
  PHPythiaContainerV2.C \
  PHPyOscarOutputManager.C \
  PHPyVertexShift.C \
  PHPythiaAncestryHelper.C \
  PHPythia_Dict.C

libPHPythiaEventGen_la_DEPENDENCIES = libPHPythia.la

libPHPythiaEventGen_la_LIBADD = \
  -lPHPythia \
  -lPHHepMC

libPHPythiaEventGen_la_SOURCES = \
  PHPythia.C \
  PHPythia8.C \
  PHPy8GenTrigger.C \
  PHPy8ParticleTrigger.C \
  PHPythiaEventGen_Dict.C


BUILT_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libPHPythiaEventGen.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

PHPythia_Dict.C: \
  PHPyOscarOutputManager.h \
  PHPythiaHeaderV1.h \
  PHPythiaHeaderV2.h \
  PHPythiaContainerV2.h \
  PHPyVertexShift.h \
  PHPythiaAncestryHelper.h \
  PHPythiaLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PHPythiaEventGen_Dict.C: \
  PHPythia.h \
  PHPythia8.h \
  PHPy8GenTrigger.h \
  PHPy8ParticleTrigger.h \
  PHPythiaEventGenLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict*
	rm -f testexternals*
