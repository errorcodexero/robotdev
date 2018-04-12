#include "XeroGPG3Robot.h"
#include "NavXAngleMeasurementDevice.h"
#include "EncoderAngleMeasurementDevice.h"
#include "MessageLogger.h"
#include <GPG3MotorController.h>
#include <Encoder.h>
#include "ServoSubsystem.h"
#include "AHRS.h"
#include "ParamsParser.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace xerolib;

XeroGPG3Robot::XeroGPG3Robot()
{
}

XeroGPG3Robot::~XeroGPG3Robot()
{
}

void XeroGPG3Robot::RobotInit()
{
	MessageLogger &logger = getMessageLogger();
	ParamsParser &params = ParamsParser::get();
	std::chrono::milliseconds delay(1);

	setupConsoleLogger();
	params.readFile("/home/pi/params.txt");

	m_drivebase_p = std::make_shared<DriveBase>(*this);

	std::shared_ptr<AHRS> navx_p = std::make_shared<AHRS>(I2C::Port::Port_0);
	while (!navx_p->IsConnected())
	{
		std::this_thread::sleep_for(delay);
	}

	while (navx_p->IsCalibrating())
	{
		std::this_thread::sleep_for(delay);
	}

	std::cout << "Calibration complete" << std::endl;
	std::cout << "Firmware version " << navx_p->GetFirmwareVersion() << std::endl;

	std::shared_ptr<frc::GPG3MotorController> left_p = std::make_shared<frc::GPG3MotorController>(0);
	std::shared_ptr<frc::GPG3MotorController> right_p = std::make_shared<frc::GPG3MotorController>(1);
	m_drivebase_p->setMotors(left_p, right_p);

	std::shared_ptr<frc::Encoder> left_enc_p = std::make_shared<frc::Encoder>(0, 1);
	std::shared_ptr<frc::Encoder> right_enc_p = std::make_shared<frc::Encoder>(2, 3);
	m_drivebase_p->setEncoders(left_enc_p, right_enc_p);

	m_drivebase_p->setNavX(navx_p);

	int32_t ticks = static_cast<int32_t>(params.getValue("drivebase:encoderPulsesPerRev") + 0.5);
	double diameter = params.getValue("drivebase:diameter");
	double width = params.getValue("drivebase:width");
	double scrub = params.getValue("drivebase:scrub");
	m_drivebase_p->setPhysicalChar(ticks, diameter, width, scrub);

	addSubsystem(m_drivebase_p);

	m_eyes_p = std::make_shared<ServoSubsystem>(*this);
	std::shared_ptr<frc::Servo> servo_p = std::make_shared<frc::Servo>(0);
	m_eyes_p->setServo(servo_p);

	addSubsystem(m_eyes_p);
}


START_ROBOT_CLASS(XeroGPG3Robot);