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
COMMON=helpers

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
# Add flags to indicate the robot we are buiding
#
ADDCXXFLAGS = -DHOLONOMIC -I../util -I../control -I../executive -I../input

#
# Deploy this file as well in the deployall make target
#
DEPLOYADD=params.txt

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
	navx_control.cpp\
	pump_control.cpp\
	talon_srx_control.cpp\
	to_roborio.cpp

include ../../../makefiles/compileapp.mk
