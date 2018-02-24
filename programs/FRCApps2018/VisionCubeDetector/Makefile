#
# Makefile for the vision application running on the Raspberry Pi
#

#
# define the target executable
#
TARGET=VisionCubeDetector

#
# Tell the make system we want the shared code (COMMON)
#
#COMMON=helpers

#
# Tell the make system we need to the WPI libraries
#
WPILIB=false

#
# Tell the make system we need the NAVX libraries
#
NAVX=false

#
# Tell the make system we need the CTRE libraries
#
CTRE=false

#
# Add build flags and libraries
#
CXXFLAGS = `pkg-config --cflags opencv`
ADDLIBS = `pkg-config --libs opencv libv4l2`


#
# define the app lib
#
APPLIBS=

#
# define the source code
#
SRC = \
	Cube.cpp\
	params_parser.cpp\
	main.cpp

include ../../../makefiles/compileapp.mk
