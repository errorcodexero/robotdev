#
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
	auto_distances.cpp\
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
	negedge_trigger.cpp\
	network.cpp\
	output.cpp\
	perf_tracker.cpp\
	persistent_tracker.cpp\
	point.cpp\
	posedge_toggle.cpp\
	posedge_trigger.cpp\
	posedge_trigger_debounce.cpp\
	settable_constant.cpp\
	settable_toggle.cpp\
	speed_tracker.cpp\
	stall_monitor.cpp\
	string_utils.cpp\
	type.cpp\
	util.cpp\
	visitable.cpp

TESTS = \
	auto_distances.cpp\
	bound_tracker.cpp\
	countdown_timer.cpp\
	countup_timer.cpp\
	fixVictor.cpp\
	input.cpp\
	motion_profile.cpp\
	negedge_trigger.cpp\
	network.cpp\
	output.cpp\
	perf_tracker.cpp\
	settable_constant.cpp\
	speed_profile.cpp\
	stall_monitor.cpp\
	string_utils.cpp\
	type.cpp\
	util.cpp

COMPLEXTESTS = 

ACOMPLEXTESTS = \
	debounce\
	visitable\
	speed_tracker\
	settable_toggle\
	posedge_trigger\
	posedge_trigger_debounce\
	point\
	persistent_tracker\
	driver_station_interface\
	interface\
	meta


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

#
# Complex tests
#
$(TESTTARGETDIR)/debounce.exe: debounce.cpp countdown_timer.cpp
$(TESTTARGETDIR)/visitable.exe: visitable.cpp type.cpp
$(TESTTARGETDIR)/speed_tracker.exe: speed_tracker.cpp countdown_timer.cpp
$(TESTTARGETDIR)/settable_toggle.exe: settable_toggle.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger.exe: posedge_trigger.cpp
$(TESTTARGETDIR)/posedge_trigger_debounce.exe: posedge_trigger_debounce.cpp countdown_timer.cpp posedge_trigger.cpp
$(TESTTARGETDIR)/point.exe: point.cpp util.cpp
$(TESTTARGETDIR)/persistent_tracker.exe: persistent_tracker.cpp util.cpp
$(TESTTARGETDIR)/driver_station_interface.exe: driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/interface.exe: interface.cpp driver_station_interface.cpp util.cpp
$(TESTTARGETDIR)/meta.exe: meta.cpp interface.cpp driver_station_interface.cpp util.cpp


