#
# makefile for the executive library
#

#
# define our target library
#
TARGET=executive.a

#
# define our source code
#
SRC = \
	autonomous.cpp\
	chain.cpp\
	executive.cpp\
	step.cpp\
	teleop.cpp\
	test.cpp\
	timed.cpp

TESTS = \
	timed.cpp

COMPLEXTESTS = \
	autonomous\
	chain\
	executive\
	step\
	teleop\
	test

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk


$(TESTTARGETDIR)/autonomous.exe: autonomous.cpp teleop.cpp step.cpp executive.cpp chain.cpp test.cpp
$(TESTTARGETDIR)/autonomous.exe: ../control/drivebase.cpp ../control/toplevel.cpp ../control/lights.cpp
$(TESTTARGETDIR)/autonomous.exe: ../control/pump.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/autonomous.exe: ../util/countdown_timer.cpp ../util/stall_monitor.cpp ../util/interface.cpp
$(TESTTARGETDIR)/autonomous.exe: ../util/driver_station_interface.cpp ../util/motion_profile.cpp ../util/posedge_trigger.cpp
$(TESTTARGETDIR)/autonomous.exe: ../util/type.cpp ../util/util.cpp
$(TESTTARGETDIR)/autonomous.exe: ../input/util.cpp ../input/panel.cpp

$(TESTTARGETDIR)/chain.exe: chain.cpp executive.cpp teleop.cpp step.cpp autonomous.cpp
$(TESTTARGETDIR)/chain.exe: ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/chain.exe: ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/chain.exe: ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/chain.exe: ../control/toplevel.cpp ../control/lights.cpp ../control/pump.cpp
$(TESTTARGETDIR)/chain.exe: ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/chain.exe: ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/executive.exe: executive.cpp
$(TESTTARGETDIR)/executive.exe: ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/executive.exe: ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/executive.exe: ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/executive.exe: ../control/toplevel.cpp ../control/lights.cpp ../control/pump.cpp
$(TESTTARGETDIR)/executive.exe: ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/executive.exe: ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/step.exe: step.cpp
$(TESTTARGETDIR)/step.exe: ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/step.exe: ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/step.exe: ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/step.exe: ../control/toplevel.cpp ../control/lights.cpp ../control/pump.cpp
$(TESTTARGETDIR)/step.exe: ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/step.exe: ../input/panel.cpp ../input/util.cpp


$(TESTTARGETDIR)/teleop.exe: teleop.cpp executive.cpp test.cpp autonomous.cpp step.cpp chain.cpp
$(TESTTARGETDIR)/teleop.exe: ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/teleop.exe: ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/teleop.exe: ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/teleop.exe: ../control/toplevel.cpp ../control/lights.cpp ../control/pump.cpp
$(TESTTARGETDIR)/teleop.exe: ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/teleop.exe: ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/test.exe: test.cpp executive.cpp teleop.cpp autonomous.cpp step.cpp chain.cpp
$(TESTTARGETDIR)/test.exe: ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/test.exe: ../util/stall_monitor.cpp ../util/type.cpp
$(TESTTARGETDIR)/test.exe: ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/test.exe: ../control/toplevel.cpp ../control/lights.cpp ../control/pump.cpp
$(TESTTARGETDIR)/test.exe: ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/test.exe: ../input/panel.cpp ../input/util.cpp

