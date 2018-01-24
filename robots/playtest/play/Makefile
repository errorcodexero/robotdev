#
# makefile for the top level robot application
#

#
# define the target executable
#
TARGET=FRCUserProgram

#
# Additional files to deploy with the target
#
DEPLOYADD = params

#
# Add flags to get to the xerolib header file
#
COMMON=xerolibtest

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
APPLIBS=

#
# Add the libs we want to add
#
USERLIBS = -lstdc++fs


#
# define the source code
#
SRC = \
	Robot2018.cpp\
	Robot2018Autonomous.cpp

include ../../../makefiles/compileapp.mk

