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
	joystick_linux.cpp\
	panel.cpp\
	util.cpp\
	pixycam/PixyBlock.cpp\
	pixycam/PixyImpl.cpp\
	pixycam/PixyTCP.cpp\
	pixycam/PixyUART.cpp

MKDIRS = pixycam

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk
