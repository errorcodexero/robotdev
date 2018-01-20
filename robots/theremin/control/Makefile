#
# makefile for the control library
#

#
# define our target library
#
TARGET=control.a

#
# define the source code files
#
SRC = \
	drivebase.cpp\
	force.cpp\
	force_interface.cpp\
	lights.cpp\
	log.cpp\
	main.cpp\
	monitor.cpp\
	motor_check.cpp\
	nop.cpp\
	pump.cpp\
	sim.cpp\
	pidctrl.cpp\
	toplevel.cpp

TESTS = \
	motor_check.cpp

COMPLEXTESTS = \
	drivebase\
	force_interface\
	lights\
	log\
	main\
	monitor\
	nop\
	pump\
	toplevel
#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/drivebase.exe: drivebase.cpp motor_check.cpp 
$(TESTTARGETDIR)/drivebase.exe: ../util/countdown_timer.cpp ../util/stall_monitor.cpp ../util/interface.cpp 
$(TESTTARGETDIR)/drivebase.exe: ../util/util.cpp ../util/point.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/force_interface.exe: force_interface.cpp force.cpp
$(TESTTARGETDIR)/force_interface.exe: ../util/interface.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/force_interface.exe: ../util/posedge_trigger.cpp ../util/util.cpp

$(TESTTARGETDIR)/lights.exe: lights.cpp nop.cpp
$(TESTTARGETDIR)/lights.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/log.exe: log.cpp nop.cpp toplevel.cpp drivebase.cpp pump.cpp lights.cpp motor_check.cpp
$(TESTTARGETDIR)/log.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/log.exe: ../util/stall_monitor.cpp ../util/countdown_timer.cpp

$(TESTTARGETDIR)/main.exe: main.cpp toplevel.cpp nop.cpp pump.cpp drivebase.cpp nop.cpp lights.cpp
$(TESTTARGETDIR)/main.exe: motor_check.cpp force_interface.cpp force.cpp log.cpp
$(TESTTARGETDIR)/main.exe: ../util/countup_timer.cpp ../util/countdown_timer.cpp ../util/stall_monitor.cpp
$(TESTTARGETDIR)/main.exe: ../util/interface.cpp ../util/posedge_trigger.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/main.exe: ../util/perf_tracker.cpp ../util/util.cpp ../util/type.cpp
$(TESTTARGETDIR)/main.exe: ../util/motion_profile.cpp
$(TESTTARGETDIR)/main.exe: ../executive/executive.cpp ../executive/teleop.cpp ../executive/autonomous.cpp
$(TESTTARGETDIR)/main.exe: ../executive/chain.cpp ../executive/step.cpp
$(TESTTARGETDIR)/main.exe: ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/nop.exe: nop.cpp
$(TESTTARGETDIR)/nop.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/monitor.exe: monitor.cpp force_interface.cpp toplevel.cpp force.cpp
$(TESTTARGETDIR)/monitor.exe: pump.cpp drivebase.cpp lights.cpp nop.cpp motor_check.cpp
$(TESTTARGETDIR)/monitor.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/monitor.exe: ../util/posedge_trigger.cpp ../util/stall_monitor.cpp ../util/countdown_timer.cpp

$(TESTTARGETDIR)/pump.exe: pump.cpp
$(TESTTARGETDIR)/pump.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/toplevel.exe: toplevel.cpp drivebase.cpp lights.cpp pump.cpp motor_check.cpp nop.cpp
$(TESTTARGETDIR)/toplevel.exe: ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/toplevel.exe: ../util/countdown_timer.cpp ../util/stall_monitor.cpp ../util/input.cpp
$(TESTTARGETDIR)/toplevel.exe: ../util/output.cpp
