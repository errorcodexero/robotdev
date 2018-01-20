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
	util.cpp\
	pixycam/PixyBlock.cpp\
	pixycam/PixyImpl.cpp\
	pixycam/PixyTCP.cpp\
	pixycam/PixyUART.cpp

TESTS = \
	util.cpp

COMPLEXTESTS = \
	panel

MKDIRS = pixycam

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/panel.exe: panel.cpp util.cpp
