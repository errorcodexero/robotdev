#
# makefile for the top level robot application
#

#
# define the target executable
#
TARGET=FRCUserProgram

#
# Tell the make system we need to the WPI libraries
#
WPILIB=true

#
# Tell the make system we need the NAVX libraries
#
NAVX=false

#
# Tell the make system we need the CTRE libraries
#
CTRE=true

#
# define the app lib
#
APPLIBS=\
	control\
	input\
	executive\
	util

#
# define the source code
#
SRC = \
	dio_control.cpp\
	talon_srx_control.cpp\
	to_roborio.cpp

include ../../../makefiles/compileapp.mk
