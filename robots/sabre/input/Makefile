#
# makefile for the control library
#

#
# define our target library
#
TARGET=input.a

#
# define the source code files
#
SRC = \
	panel.cpp\
	util.cpp

TESTS = \

COMPLEXTESTS = \
	panel\
	util


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/panel$(EXEEXT): panel.cpp util.cpp
$(TESTTARGETDIR)/panel$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/util$(EXEEXT): util.cpp ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp ../util/point.cpp
