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
	climb_release.cpp\
	collector.cpp\
	tilt.cpp\
	front.cpp\
	sides.cpp\
	shooter.cpp\
	winch.cpp\
	force.cpp\
	force_interface.cpp\
	log.cpp\
	main.cpp\
	monitor.cpp\
	motor_check.cpp\
	nop.cpp\
	pump.cpp\
	shooter_constants.cpp\
	sim.cpp\
	tilt_presets.cpp\
	toplevel.cpp

ifndef TESTS
TESTS = \
	motor_check\
	tilt_presets
endif

ifndef COMPLEXTESTS
COMPLEXTESTS = \
	drivebase\
	climb_release\
	collector\
	front\
	sides\
	shooter\
	winch\
	force_interface\
	log\
	main\
	monitor\
	nop\
	pump\
	shooter_constants\
	toplevel
endif

#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/drivebase$(EXEEXT): drivebase.cpp motor_check.cpp 
$(TESTTARGETDIR)/drivebase$(EXEEXT): ../util/countdown_timer.cpp ../util/interface.cpp 
$(TESTTARGETDIR)/drivebase$(EXEEXT): ../util/util.cpp ../util/point.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/climb_release$(EXEEXT): climb_release.cpp 
$(TESTTARGETDIR)/climb_release$(EXEEXT): ../util/countdown_timer.cpp ../util/interface.cpp 
$(TESTTARGETDIR)/climb_release$(EXEEXT): ../util/util.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/collector$(EXEEXT): collector.cpp tilt.cpp front.cpp sides.cpp 
$(TESTTARGETDIR)/collector$(EXEEXT): ../util/util.cpp ../util/interface.cpp nop.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/front$(EXEEXT): front.cpp 
$(TESTTARGETDIR)/front$(EXEEXT): ../util/interface.cpp ../util/util.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/sides$(EXEEXT): sides.cpp 
$(TESTTARGETDIR)/sides$(EXEEXT): nop.cpp ../util/interface.cpp ../util/util.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/tilt$(EXEEXT): tilt.cpp 
$(TESTTARGETDIR)/tilt$(EXEEXT): ../util/countdown_timer.cpp ../util/interface.cpp ../util/util.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/shooter$(EXEEXT): shooter.cpp 
$(TESTTARGETDIR)/shooter$(EXEEXT): ../util/interface.cpp ../util/util.cpp 
$(TESTTARGETDIR)/shooter$(EXEEXT): ../util/countdown_timer.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/winch$(EXEEXT): winch.cpp 
$(TESTTARGETDIR)/winch$(EXEEXT): ../util/countdown_timer.cpp ../util/interface.cpp ../util/util.cpp ../util/driver_station_interface.cpp

$(TESTTARGETDIR)/force_interface$(EXEEXT): force_interface.cpp force.cpp
$(TESTTARGETDIR)/force_interface$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/force_interface$(EXEEXT): ../util/posedge_trigger.cpp ../util/util.cpp

$(TESTTARGETDIR)/log$(EXEEXT): log.cpp nop.cpp toplevel.cpp drivebase.cpp collector.cpp tilt.cpp front.cpp sides.cpp shooter.cpp winch.cpp climb_release.cpp pump.cpp motor_check.cpp
$(TESTTARGETDIR)/log$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/log$(EXEEXT): ../util/countdown_timer.cpp

$(TESTTARGETDIR)/main$(EXEEXT): main.cpp toplevel.cpp nop.cpp pump.cpp drivebase.cpp collector.cpp tilt.cpp sides.cpp front.cpp winch.cpp shooter.cpp climb_release.cpp
$(TESTTARGETDIR)/main$(EXEEXT): motor_check.cpp force_interface.cpp force.cpp log.cpp nop.cpp tilt_presets.cpp shooter_constants.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/countup_timer.cpp ../util/countdown_timer.cpp ../util/point.cpp ../util/debounce.cpp 
$(TESTTARGETDIR)/main$(EXEEXT): ../util/interface.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/driver_station_interface.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/perf_tracker.cpp ../util/util.cpp ../util/type.cpp ../util/point.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../util/motion_profile.cpp ../util/nav2.cpp ../util/nav.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/executive.cpp ../executive/teleop.cpp ../executive/delay.cpp 
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/step.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/auto_null.cpp ../executive/auto_reach.cpp ../executive/auto_static.cpp ../executive/auto_statictwo.cpp 
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/auto_stop.cpp ../executive/auto_test.cpp ../executive/barrelracer.cpp ../executive/cheval.cpp
$(TESTTARGETDIR)/main$(EXEEXT): ../executive/low_bar_low_score.cpp ../executive/low_bar_wall_high_score.cpp ../executive/low_bar_wall_low_score.cpp ../executive/portcullis.cpp

$(TESTTARGETDIR)/main$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/nop$(EXEEXT): nop.cpp
$(TESTTARGETDIR)/nop$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/monitor$(EXEEXT): monitor.cpp force_interface.cpp toplevel.cpp force.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): pump.cpp drivebase.cpp collector.cpp sides.cpp front.cpp tilt.cpp shooter.cpp winch.cpp climb_release.cpp nop.cpp motor_check.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/monitor$(EXEEXT): ../util/posedge_trigger.cpp ../util/countdown_timer.cpp

$(TESTTARGETDIR)/pump$(EXEEXT): pump.cpp
$(TESTTARGETDIR)/pump$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/shooter_constants$(EXEEXT): shooter_constants.cpp
$(TESTTARGETDIR)/shooter_constants$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp

$(TESTTARGETDIR)/toplevel$(EXEEXT): toplevel.cpp drivebase.cpp collector.cpp front.cpp sides.cpp tilt.cpp winch.cpp shooter.cpp climb_release.cpp pump.cpp motor_check.cpp nop.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/interface.cpp ../util/driver_station_interface.cpp ../util/util.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/countdown_timer.cpp ../util/input.cpp
$(TESTTARGETDIR)/toplevel$(EXEEXT): ../util/output.cpp
