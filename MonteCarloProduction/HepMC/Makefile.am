AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(includedir) -I$(OFFLINE_MAIN)/include -I`root-config --incdir` -I$(HEPMC)/include 
AM_LDFLAGS = -L$(libdir) -L$(OFFLINE_MAIN)/lib -L$(OPT_PHENIX)/lib -L$(HEPMC)/lib -L/usr/lib 

AM_CXXFLAGS = -Wall -Werror

lib_LTLIBRARIES = libPHHepMC.la 

include_HEADERS = \
  PHHepMCGenEvent.h \
  PHHepMCHelper.h



libPHHepMC_la_LDFLAGS = ${AM_LDFLAGS} `root-config --libs`
libPHHepMC_la_LIBADD = \
  -lphool -lSubsysReco -lfun4all -lz -lboost_iostreams -lHepMC -lHepMCfio -lEG

libPHHepMC_la_SOURCES = \
  PHHepMCGenEvent.C \
  PHHepMCHelper.C \
  PHHepMC_Dict.C

BUILT_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals


testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

PHHepMC_Dict.C: \
  PHHepMCGenEvent.h \
  PHHepMCHelper.h \
  $(HEPMC)/include/HepMC/*.h \
  PHHepMCLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict*
	rm -f testexternals*
