#include "XeroGPG3Robot.h"
#include "RobotGPG3Consts.h"
#include "NavXAngleMeasurementDevice.h"
#include "EncoderAngleMeasurementDevice.h"
#include "MessageLogger.h"
#include <GPG3MotorController.h>
#include <Encoder.h>
#include "ServoSubsystem.h"
#include "AHRS.h"
#include "ParamFileReader.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace xerolib;

DriveBase::MotionProfileParams XeroGPG3Robot::StraightParams;
DriveBase::MotionProfileParams XeroGPG3Robot::RotationalParams;
DriveBase::PIDConstants XeroGPG3Robot::StraightPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };
DriveBase::PIDConstants XeroGPG3Robot::DistancePIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants XeroGPG3Robot::AngleCorrPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants XeroGPG3Robot::RotationalPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants XeroGPG3Robot::AnglePIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
double XeroGPG3Robot::Diameter;
double XeroGPG3Robot::DistanceThreshold;
double XeroGPG3Robot::DistanceSwitchThreshold;
double XeroGPG3Robot::AngleThreshold;
double XeroGPG3Robot::AngleSwitchThreshold;
int32_t XeroGPG3Robot::TicksPerRev;
double XeroGPG3Robot::ErrorThreshold;

const std::string XeroGPG3Robot::ParamDiameter("drivebase:diameter");
const std::string XeroGPG3Robot::ParamStraightThreshold("drivebase:straight:threshold");
const std::string XeroGPG3Robot::ParamStraightSwitchThreshold("drivebase:straight:switch:threshold");
const std::string XeroGPG3Robot::ParamAngleThreshold("drivebase:angle:threshold");
const std::string XeroGPG3Robot::ParamAngleSwitchThreshold("drivebase:angle:switch:threshold");
const std::string XeroGPG3Robot::ParamErrorThreshold("drivebase:error:threshold");
const std::string XeroGPG3Robot::ParamPulsesPerRev("drivebase:encoderPulsesPerRev");

const std::string XeroGPG3Robot::ParamStraightAccel("drivebase:straight:accel");
const std::string XeroGPG3Robot::ParamStraightDecel("drivebase:straight:decel");
const std::string XeroGPG3Robot::ParamStraightMaxSpeed("drivebase:straight:maxspeed");
const std::string XeroGPG3Robot::ParamStraightMaxAge("drivebase:straight:maxage");
const std::string XeroGPG3Robot::ParamStraightP("drivebase:straight:p");
const std::string XeroGPG3Robot::ParamStraightI("drivebase:straight:i");
const std::string XeroGPG3Robot::ParamStraightD("drivebase:straight:d");
const std::string XeroGPG3Robot::ParamStraightF("drivebase:straight:f");
const std::string XeroGPG3Robot::ParamStraightIMax("drivebase:straight:imax");
const std::string XeroGPG3Robot::ParamDistanceP("drivebase:distance:p");
const std::string XeroGPG3Robot::ParamDistanceI("drivebase:distance:i");
const std::string XeroGPG3Robot::ParamDistanceD("drivebase:distance:d");
const std::string XeroGPG3Robot::ParamDistanceF("drivebase:distance:f");
const std::string XeroGPG3Robot::ParamDistanceIMax("drivebase:distance:imax");
const std::string XeroGPG3Robot::ParamAngleCorrP("drivebase:angleCorrection:p");
const std::string XeroGPG3Robot::ParamAngleCorrI("drivebase:angleCorrection:i");
const std::string XeroGPG3Robot::ParamAngleCorrD("drivebase:angleCorrection:d");
const std::string XeroGPG3Robot::ParamAngleCorrF("drivebase:angleCorrection:f");
const std::string XeroGPG3Robot::ParamAngleCorrIMax("drivebase:angleCorrection:imax");

const std::string XeroGPG3Robot::ParamAngleAccel("drivebase:angle:accel");
const std::string XeroGPG3Robot::ParamAngleDecel("drivebase:angle:decel");
const std::string XeroGPG3Robot::ParamAngleMaxSpeed("drivebase:angle:maxspeed");
const std::string XeroGPG3Robot::ParamAngleMaxAge("drivebase:angle:maxage");
const std::string XeroGPG3Robot::ParamRotationalP("drivebase:rotational:p");
const std::string XeroGPG3Robot::ParamRotationalI("drivebase:rotational:i");
const std::string XeroGPG3Robot::ParamRotationalD("drivebase:rotational:d");
const std::string XeroGPG3Robot::ParamRotationalF("drivebase:rotational:f");
const std::string XeroGPG3Robot::ParamRotationalIMax("drivebase:rotational:imax");
const std::string XeroGPG3Robot::ParamAngleP("drivebase:angle:p");
const std::string XeroGPG3Robot::ParamAngleI("drivebase:angle:i");
const std::string XeroGPG3Robot::ParamAngleD("drivebase:angle:d");
const std::string XeroGPG3Robot::ParamAngleF("drivebase:angle:f");
const std::string XeroGPG3Robot::ParamAngleIMax("drivebase:angle:imax");


XeroGPG3Robot::XeroGPG3Robot()
{
}

XeroGPG3Robot::~XeroGPG3Robot()
{
}

void XeroGPG3Robot::extractParams()
{
	TicksPerRev = static_cast<int32_t>(std::round(getParam(ParamPulsesPerRev, 200)));
	Diameter = getParam(ParamDiameter, 5.0);
	DistanceThreshold = getParam(ParamStraightThreshold, 1.0);
	DistanceSwitchThreshold = getParam(ParamStraightSwitchThreshold, 1.0);
	AngleSwitchThreshold = getParam(ParamAngleSwitchThreshold, 1.0);
	AngleThreshold = getParam(ParamAngleThreshold, 5.0);
	ErrorThreshold = getParam(ParamErrorThreshold, 6.0);

	StraightParams.acceleration = getParam(ParamStraightAccel, StraightParams.acceleration);
	StraightParams.deceleration = getParam(ParamStraightDecel, StraightParams.deceleration);
	StraightParams.maxspeed = getParam(ParamStraightMaxSpeed, StraightParams.maxspeed);
	StraightParams.maxage = getParam(ParamStraightMaxAge, StraightParams.maxage);

	StraightPIDConstants.p = getParam(ParamStraightP, StraightPIDConstants.p);
	StraightPIDConstants.i = getParam(ParamStraightI, StraightPIDConstants.i);
	StraightPIDConstants.d = getParam(ParamStraightD, StraightPIDConstants.d);
	StraightPIDConstants.f = getParam(ParamStraightF, StraightPIDConstants.f);
	StraightPIDConstants.imax = getParam(ParamStraightIMax, StraightPIDConstants.imax);

	DistancePIDConstants.p = getParam(ParamDistanceP, DistancePIDConstants.p);
	DistancePIDConstants.i = getParam(ParamDistanceI, DistancePIDConstants.i);
	DistancePIDConstants.d = getParam(ParamDistanceD, DistancePIDConstants.d);
	DistancePIDConstants.f = getParam(ParamDistanceF, DistancePIDConstants.f);
	DistancePIDConstants.imax = getParam(ParamDistanceIMax, DistancePIDConstants.imax);

	AngleCorrPIDConstants.p = getParam(ParamAngleCorrP, AngleCorrPIDConstants.p);
	AngleCorrPIDConstants.i = getParam(ParamAngleCorrI, AngleCorrPIDConstants.i);
	AngleCorrPIDConstants.d = getParam(ParamAngleCorrD, AngleCorrPIDConstants.d);
	AngleCorrPIDConstants.f = getParam(ParamAngleCorrF, AngleCorrPIDConstants.f);
	AngleCorrPIDConstants.imax = getParam(ParamAngleCorrIMax, AngleCorrPIDConstants.imax);

	RotationalParams.acceleration = getParam(ParamAngleAccel, RotationalParams.acceleration);
	RotationalParams.deceleration = getParam(ParamStraightDecel, RotationalParams.deceleration);
	RotationalParams.maxspeed = getParam(ParamAngleMaxSpeed, RotationalParams.maxspeed);
	RotationalParams.maxage = getParam(ParamAngleMaxAge, RotationalParams.maxage);

	RotationalPIDConstants.p = getParam(ParamRotationalP, RotationalPIDConstants.p);
	RotationalPIDConstants.i = getParam(ParamRotationalI, RotationalPIDConstants.i);
	RotationalPIDConstants.d = getParam(ParamRotationalD, RotationalPIDConstants.d);
	RotationalPIDConstants.f = getParam(ParamRotationalF, RotationalPIDConstants.f);
	RotationalPIDConstants.imax = getParam(ParamRotationalIMax, RotationalPIDConstants.imax);

	AnglePIDConstants.p = getParam(ParamAngleP, AnglePIDConstants.p);
	AnglePIDConstants.i = getParam(ParamAngleI, AnglePIDConstants.i);
	AnglePIDConstants.d = getParam(ParamAngleD, AnglePIDConstants.d);
	AnglePIDConstants.f = getParam(ParamAngleF, AnglePIDConstants.f);
	AnglePIDConstants.imax = getParam(ParamAngleIMax, AnglePIDConstants.imax);
}

void XeroGPG3Robot::dumpParams()
{
	MessageLogger &logger = getMessageLogger();
	logger << MessageLogger::MessageType::Info;
	logger << "--------------------------------------------------------------------------";
	logger << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info;
	logger << "Drivebase Initialization Parameters";
	logger << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info;
	logger << "--------------------------------------------------------------------------";
	logger << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamPulsesPerRev << " " << TicksPerRev << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamDiameter << " " << Diameter << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightThreshold << " " << DistanceThreshold << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightSwitchThreshold << " " << DistanceSwitchThreshold << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleSwitchThreshold << " " << AngleSwitchThreshold << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleThreshold << " " << AngleThreshold << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamErrorThreshold << " " << ErrorThreshold << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamStraightAccel << " " << StraightParams.acceleration << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightDecel << " " << StraightParams.deceleration << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightMaxSpeed << " " << StraightParams.maxspeed << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightMaxAge << " " << StraightParams.maxage << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamStraightP << " " << StraightPIDConstants.p << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightI << " " << StraightPIDConstants.i << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightD << " " << StraightPIDConstants.d << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightF << " " << StraightPIDConstants.f << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightIMax << " " << StraightPIDConstants.imax << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamDistanceP << " " << DistancePIDConstants.p << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamDistanceI << " " << DistancePIDConstants.i << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamDistanceD << " " << DistancePIDConstants.d << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamDistanceF << " " << DistancePIDConstants.f << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamDistanceIMax << " " << DistancePIDConstants.imax << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamAngleCorrP << " " << AngleCorrPIDConstants.p << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleCorrI << " " << AngleCorrPIDConstants.i << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleCorrD << " " << AngleCorrPIDConstants.d << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleCorrF << " " << AngleCorrPIDConstants.f << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleCorrIMax << " " << AngleCorrPIDConstants.imax << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamAngleAccel << " " << RotationalParams.acceleration << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamStraightDecel << " " << RotationalParams.deceleration << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleMaxSpeed << " " << RotationalParams.maxspeed << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleMaxAge << " " << RotationalParams.maxage << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamRotationalP << " " << RotationalPIDConstants.p << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamRotationalI << " " << RotationalPIDConstants.i << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamRotationalD << " " << RotationalPIDConstants.d << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamRotationalF << " " << RotationalPIDConstants.f << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamRotationalIMax << " " << RotationalPIDConstants.imax << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info << ParamAngleP << " " << AnglePIDConstants.p << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleI << " " << AnglePIDConstants.i << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleD << " " << AnglePIDConstants.d << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleF << " " << AnglePIDConstants.f << MessageLogger::Token::EndOfMessage;
	logger << MessageLogger::MessageType::Info << ParamAngleIMax << " " << AnglePIDConstants.imax << MessageLogger::Token::EndOfMessage;

	logger << MessageLogger::MessageType::Info;
	logger << "--------------------------------------------------------------------------";
	logger << MessageLogger::Token::EndOfMessage;
}

void XeroGPG3Robot::RobotInit()
{
	MessageLogger &logger = getMessageLogger();

	setupLoggers("robot.log", "robot.csv");
	setupConsoleLogger();
	setupTcpLogger("192.168.1.85", 8888);

	readParams();
	extractParams();
	dumpParams();

	m_drivebase_p = std::make_shared<DriveBase>(*this);

	std::shared_ptr<frc::GPG3MotorController> left_p = std::make_shared<frc::GPG3MotorController>(0);
	std::shared_ptr<frc::GPG3MotorController> right_p = std::make_shared<frc::GPG3MotorController>(1);
	m_drivebase_p->setMotors(left_p, right_p);

	std::shared_ptr<frc::Encoder> left_enc_p = std::make_shared<frc::Encoder>(0, 1);
	std::shared_ptr<frc::Encoder> right_enc_p = std::make_shared<frc::Encoder>(2, 3);

	m_drivebase_p->setEncoders(left_enc_p, right_enc_p);


	std::shared_ptr<AngleMeasurementDevice> angle_measure_p;

	try
	{
		angle_measure_p = std::make_shared<NavXAngleMeasurementDevice>();
	}
	catch (const std::exception &ex)
	{
		angle_measure_p = std::make_shared<EncoderAngleMeasurementDevice>(left_enc_p, right_enc_p);
	}

	m_drivebase_p->setAngleMeasurementDevice(angle_measure_p);


	m_drivebase_p->setPhysicalChar(TicksPerRev, Diameter);
	m_drivebase_p->setStraightMotionProfiles(StraightParams, DistanceThreshold, 10000.0);
	m_drivebase_p->setStraightPIDConstants(StraightPIDConstants, DistancePIDConstants, AngleCorrPIDConstants);
	m_drivebase_p->setRotationalMotionProfiles(RotationalParams, AngleThreshold, 10000.0);
	m_drivebase_p->setRotationalPIDConstants(RotationalPIDConstants, AngleCorrPIDConstants);
	m_drivebase_p->setErrorThreshold(ErrorThreshold);

	addSubsystem(m_drivebase_p);

	m_eyes_p = std::make_shared<ServoSubsystem>(*this);
	std::shared_ptr<frc::Servo> servo_p = std::make_shared<frc::Servo>(0);
	m_eyes_p->setServo(servo_p);

	addSubsystem(m_eyes_p);
}


START_ROBOT_CLASS(XeroGPG3Robot);