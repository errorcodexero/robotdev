#
# makefile for the top level robot application
#

#
# define the target executable
#
TARGET=FRCUserProgram

#
# Tell the make system we want the shared code (COMMON)
#
COMMON=util

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
	control.a\
	input.a\
	executive.a\
	util.a

#
# define the source code
#
SRC = \
	dio_control.cpp\
	talon_srx_control.cpp\
	to_roborio.cpp

include ../../../makefiles/compileapp.mk
