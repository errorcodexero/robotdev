#
# makefile for the control library
#

#
# define our target library
#
TARGET=xerolib.a

#
# Define the external software we need
#
WPILIB=true

NAVX=true

#
# define the source code files
#
SRC = \
	AutoDelay.cpp\
	AutoDriveBaseChar.cpp\
	AutoDriveDistance.cpp\
	AutonomousControllerBase.cpp\
	AutonomousStepBase.cpp\
	AutoSetMotorVoltage.cpp\
	DataDashboardDestination.cpp\
	DataFileDestination.cpp\
	DataLogger.cpp\
	DataFileDestination.cpp\
	DataTcpDestination.cpp\
	DataUDPDestination.cpp\
	DataDashboardDestination.cpp\
	DriveBase.cpp\
	EncoderAngleMeasurementDevice.cpp\
	FileManager.cpp\
	MessageLogger.cpp\
	MessageStreamDestination.cpp\
	NavXAngleMeasurementDevice.cpp\
	OperatorControllerBase.cpp\
	ParamFileReader.cpp\
	PIDCtrl.cpp\
	RobotControllerBase.cpp\
	SubsystemBase.cpp\
	TrapezoidSpeedProfile.cpp\
	XeroRobotBase.cpp


#
# include a central make file that defines our compilation recipe
#
include ../../../makefiles/compilelib.mk

