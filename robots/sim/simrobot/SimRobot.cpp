#include "SimRobot.h"

//
// Misc general stuff
//
#include <ModuleDefintions.h>
#include <ParamsParser.h>
#include <MessageLogger.h>

//
// HW objects
//
#include <SimMotorController.h>
#include <AHRS.h>

//
// Subsytems and supporting controllers
//
#include <TankDrive.h>
#include <DistanceFollower.h>
#include <KalmanOdometry.h>
#include <IntelligentDriveBase.h>

#ifdef SIMULATOR
#include <RobotSimulator.h>
#endif

using namespace xero::base;

SimRobot::SimRobot()
{
}

SimRobot::~SimRobot()
{
}

std::shared_ptr<DriveBase> SimRobot::createDriveBase()
{
	std::shared_ptr<TankDrive> drive_p;
	std::chrono::milliseconds delay(1);

	auto &params = xero::base::ParamsParser::get();

	std::shared_ptr<AHRS> navx_p = std::make_shared<AHRS>("/dev/ttyACM0");
	while (!navx_p->IsConnected())
		std::this_thread::sleep_for(delay);

	std::shared_ptr<frc::SimMotorController> left_p = std::make_shared<frc::SimMotorController>(0);
	std::shared_ptr<frc::SimMotorController> right_p = std::make_shared<frc::SimMotorController>(1);
	std::shared_ptr<frc::Encoder> left_enc_p = std::make_shared<frc::Encoder>(0, 1);
	std::shared_ptr<frc::Encoder> right_enc_p = std::make_shared<frc::Encoder>(2, 3);
	drive_p = std::make_shared<TankDrive>(*this, left_p, right_p, left_enc_p, right_enc_p, navx_p);

	int32_t ticks = static_cast<int32_t>(params.getValue("TankDrive:ticks_per_rev") + 0.5);
	double diameter = params.getValue("TankDrive:diameter");
	double width = params.getValue("TankDrive:width");
	double scrub = params.getValue("TankDrive:scrub");

	drive_p->setPhysicalChar(ticks, diameter, width, scrub);

	return drive_p;
}

std::shared_ptr<OdometrySystem> SimRobot::createOdometryController()
{
	std::shared_ptr<KalmanOdometry> odom_p = std::make_shared<KalmanOdometry>();
	return odom_p;
}

std::shared_ptr<PathFollowerController> SimRobot::createFollower()
{
	xero::base::DistanceFollower::FollowConfig config;

	auto &params = xero::base::ParamsParser::get();

	config.p = params.getValue("TankDrive:follower:p");
	config.d = params.getValue("TankDrive:follower:d");
	config.v = params.getValue("TankDrive:follower:v");
	config.a = params.getValue("TankDrive:follower:a");
	std::shared_ptr<DistanceFollower> follower_p = std::make_shared<DistanceFollower>(config);

	return follower_p;
}

void SimRobot::RobotInit()
{
	//std::string basedir("C:\\cygwin64\\home\\ButchGriffin\\src\\robot\\robotdev\\robots\\sim\\simrobot\\data");
	std::string basedir("D:\\cygwin\\home\\ButchGriffin\\src\\robotdev\\robots\\sim\\simrobot\\data");

#ifdef SIMULATOR
	frc::RobotSimulator &sim = frc::RobotSimulator::get();
	std::string logfile = basedir + "\\sim.csv";
	sim.setLogFile(logfile);
#endif

	setBaseDir(basedir.c_str());
	xero::base::XeroRobotBase::RobotInit();

	std::shared_ptr<DriveBase> drive_p = createDriveBase();
	std::shared_ptr<OdometrySystem> odometry_p = createOdometryController();
	std::shared_ptr<PathFollowerController> follower_p = createFollower();
	std::shared_ptr<IntelligentDriveBase> idrive_p = std::make_shared<IntelligentDriveBase>(*this, drive_p, odometry_p, follower_p);

	addSubsystem(idrive_p);
}

START_ROBOT_CLASS(SimRobot);