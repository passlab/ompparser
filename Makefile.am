# Makefile.am for used with REX/ROSE compiler such that libompparser.la is built and linked into librose lib.
# ompparser is a submodule of REX/ROSE under folder src/frontend/SageIII and the source files of ompparser
# are built into libompparser.la, which then be added to libsage3.la which is added to librose.so. 
#
# This file is not needed to build standalone ompparser 
# 
include $(top_srcdir)/config/Makefile.for.ROSE.includes.and.libs

AM_CPPFLAGS = -I$(top_srcdir)/src/frontend/SageIII/ompparser/src -I.

AM_CXXFLAGS = -std=c++11

omplexer.cc: $(top_srcdir)/src/frontend/SageIII/ompparser/src/omplexer.ll
	flex --outfile $@ $<

# Make sure ompparser.hh is generated before compiling omplexer.cc
omplexer.lo: ompparser.hh omplexer.cc
ompparser.hh: ompparser.cc

noinst_LTLIBRARIES = libompparser.la
libompparser_la_SOURCES = omplexer.cc src/ompparser.yy src/OpenMPIR.cpp src/OpenMPIRToString.cpp src/OpenMPIRToDOT.cpp

pkginclude_HEADERS =  src/OpenMPIR.h src/OpenMPKinds.h

clean-local:
	rm -f *.la  omplexer.cc ompparser.cc ompparser.hh
