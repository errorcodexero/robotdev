
# makefile for the misc library
#

#
# define our target library
#
TARGET=util.a

#
# define the source code files
#
SRC = \
	bound_tracker.cpp\
	countdown_timer.cpp\
	countup_timer.cpp\
	debounce.cpp\
	driver_station_interface.cpp\
	fixVictor.cpp\
	input.cpp\
	interface.cpp\
	meta.cpp\
	motion_profile.cpp\
	nav.cpp\
	nav2.cpp\
	negedge_trigger.cpp\
	network.cpp\
	output.cpp\
	perf_tracker.cpp\
	point.cpp\
	posedge_toggle.cpp\
	posedge_trigger.cpp\
	posedge_trigger_debounce.cpp\
	settable_toggle.cpp\
	string_utils.cpp\
	type.cpp\
	util.cpp\
	visitable.cpp

ifndef TESTS
TESTS = \
	bound_tracker\
	countdown_timer\
	countup_timer\
	fixVictor\
	input\
	motion_profile\
	nav\
	negedge_trigger\
	network\
	output\
	perf_tracker\
	string_utils\
	type\
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	debounce\
	nav2\
	visitable\
	settable_toggle\
	posedge_trigger\
	posedge_trigger_debounce\
	point\
	driver_station_interface\
	interface\
	meta\
	util
endif

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

#
# Complex tests
#
$(TESTTARGETDIR)/debounce$(EXEEXT): debounce.cpp countdown_timer.cpp
$(TESTTARGETDIR)/nav2$(EXEEXT): nav2.cpp nav.cpp
$(TESTTARGETDIR)/visitable$(EXEEXT): visitable.cpp type.cpp
$(TESTTARGETDIR)/settable_toggle$(EXEEXT): settable_toggle.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger$(EXEEXT): posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger_debounce$(EXEEXT): posedge_trigger_debounce.cpp debounce.cpp countdown_timer.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/point$(EXEEXT): point.cpp util.cpp
$(TESTTARGETDIR)/driver_station_interface$(EXEEXT): driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/interface$(EXEEXT): interface.cpp driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/meta$(EXEEXT): meta.cpp interface.cpp driver_station_interface.cpp util.cpp ../control/front.cpp ../control/tilt.cpp
$(TESTTARGETDIR)/utilmeta$(EXEEXT): util.cpp interface.cpp driver_station_interface.cpp 
$(TESTTARGETDIR)/util$(EXEEXT): util.cpp point.cpp interface.cpp driver_station_interface.cpp 

