#
# makefile for the control library
#

#
# define our target library
#
TARGET=input.a

#
# Tell the make system we want the shared code (COMMON)
#
COMMON=helpers

#
# Add flags to indicate the robot we are building
#
ADDCXXFLAGS = -DTHEREMIN -I../util

#
# define the source code files
#
SRC = \
	panel.cpp\
	iutil.cpp

ifndef TESTS
TESTS = \
	iutil
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	panel
endif


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/panel$(EXEEXT): panel.cpp util.cpp
