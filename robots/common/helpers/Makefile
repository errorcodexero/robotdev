#
# makefile for the misc library
#

#
# define our target library
#
TARGET=helpers.a

#
# define the source code files
#
SRC = \
	params_parser.cpp\
	udp_sync.cpp\
	message_logger.cpp\
	motion_target_profile.cpp\
	data_logger.cpp\
	data_logger_destination_server.cpp

# message_logger.cpp

ifndef TESTS
TESTS = \
	data_logger_destination_server\
	motion_target_profile
endif

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk
