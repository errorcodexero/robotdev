#
# makefile for the top level robot application
#

#
# define the target executable
#
TARGET=FRCUserProgram

#
# Add flags to get to the xerolib header file
#
ADDCXXFLAGS=-I../xerolib

#
# Tell the make system we need to the WPI libraries
#
WPILIB=true

#
# Tell the make system we need the NAVX libraries
#
NAVX=true

#
# Tell the make system we need the CTRE libraries
#
CTRE=true

#
# define the app lib
#
APPLIBS=\
	xerolib.a

#
# define the source code
#
SRC = \
	Robot2018.cpp\
	Robot2018Consts.cpp\
	Robot2018Autonomous.cpp

include ../../../makefiles/compileapp.mk

