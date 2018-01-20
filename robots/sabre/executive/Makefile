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
	auto_null.cpp\
	auto_reach.cpp\
	auto_static.cpp\
	auto_statictwo.cpp\
	auto_stop.cpp\
	auto_test.cpp\
	barrelracer.cpp\
	cheval.cpp\
	executive.cpp\
	low_bar_low_score.cpp\
	low_bar_wall_high_score.cpp\
	low_bar_wall_low_score.cpp\
	portcullis.cpp\
	step.cpp\
	teleop.cpp\
	test.cpp\
	timed.cpp

TESTS = \
	timed.cpp

COMPLEXTESTS = \
	auto_null\
	auto_reach\
	auto_static\
	auto_statictwo\
	auto_stop\
	auto_test\
	barrelracer\
	cheval\
	executive\
	low_bar_low_score\
	low_bar_wall_high_score\
	low_bar_wall_low_score\
	portcullis\
	step\
	teleop\
	test
#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

$(TESTTARGETDIR)/auto_null$(EXEEXT): auto_null.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_null$(EXEEXT): auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_null$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/auto_reach$(EXEEXT): auto_reach.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_reach$(EXEEXT): auto_null.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_reach$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/auto_static$(EXEEXT): auto_static.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_static$(EXEEXT): auto_null.cpp auto_reach.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_static$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): auto_statictwo.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_statictwo$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/auto_stop$(EXEEXT): auto_stop.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_stop$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_stop$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/auto_test$(EXEEXT): auto_test.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/auto_test$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): auto_stop.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/auto_test$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/barrelracer$(EXEEXT): barrelracer.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/barrelracer$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): auto_stop.cpp auto_test.cpp cheval.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/barrelracer$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/cheval$(EXEEXT): cheval.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/cheval$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/cheval$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/cheval$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): low_bar_low_score.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/low_bar_low_score$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): low_bar_wall_high_score.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): low_bar_low_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/low_bar_wall_high_score$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): low_bar_wall_low_score.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp portcullis.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/low_bar_wall_low_score$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/portcullis$(EXEEXT): portcullis.cpp executive.cpp teleop.cpp test.cpp delay.cpp  
$(TESTTARGETDIR)/portcullis$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/portcullis$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/executive$(EXEEXT): executive.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/type.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/executive$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/step$(EXEEXT): step.cpp executive.cpp teleop.cpp delay.cpp 
$(TESTTARGETDIR)/step$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/step$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/step$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/step$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/step$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/teleop$(EXEEXT): teleop.cpp executive.cpp test.cpp delay.cpp step.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/teleop$(EXEEXT): ../input/panel.cpp ../input/util.cpp

$(TESTTARGETDIR)/test$(EXEEXT): test.cpp executive.cpp teleop.cpp delay.cpp step.cpp
$(TESTTARGETDIR)/test$(EXEEXT): auto_null.cpp auto_reach.cpp auto_static.cpp auto_statictwo.cpp
$(TESTTARGETDIR)/test$(EXEEXT): auto_stop.cpp auto_test.cpp barrelracer.cpp cheval.cpp
$(TESTTARGETDIR)/test$(EXEEXT): low_bar_low_score.cpp low_bar_wall_high_score.cpp low_bar_wall_low_score.cpp portcullis.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/interface.cpp ../util/countdown_timer.cpp ../util/util.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/type.cpp ../util/posedge_trigger.cpp ../util/posedge_trigger_debounce.cpp ../util/debounce.cpp ../util/posedge_toggle.cpp 
$(TESTTARGETDIR)/test$(EXEEXT): ../control/tilt_presets.cpp ../control/shooter_constants.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../util/driver_station_interface.cpp ../util/posedge_trigger.cpp ../util/motion_profile.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../control/toplevel.cpp ../control/pump.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../control/drivebase.cpp ../control/nop.cpp ../control/motor_check.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../control/winch.cpp ../control/climb_release.cpp ../control/shooter.cpp ../control/front.cpp ../control/sides.cpp ../control/tilt.cpp ../control/collector.cpp
$(TESTTARGETDIR)/test$(EXEEXT): ../input/panel.cpp ../input/util.cpp
