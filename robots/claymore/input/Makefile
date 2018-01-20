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
	util.cpp

COMPLEXTESTS = \
	panel


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/panel$(EXEEXT): panel.cpp util.cpp
