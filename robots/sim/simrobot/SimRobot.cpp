#include "SimRobot.h"
#include <ModuleDefintions.h>
#include <ParamsParser.h>
#include <MessageLogger.h>
#include <DriveBase.h>
#include <AHRS.h>
#include <SimMotorController.h>
#include <RobotStateEstimator.h>

using namespace xerolib;

SimRobot::SimRobot()
{
}

SimRobot::~SimRobot()
{
}

void SimRobot::RobotInit()
{
	std::chrono::milliseconds delay(1);

	setBaseDir("D:\\cygwin\\home\\ButchGriffin\\src\\robotdev\\robots\\sim\\simrobot");

	xero::pathfinder::RobotStateEstimator::LogFileName = getBaseDir() + "\\estimator.csv";

	MessageLogger &logger = getMessageLogger();
	logger.enableModules(MODULE_ALL);
	setupConsoleLogger();
	setupFileLogger(getBaseDir() + "\\robot.log");

	ParamsParser &params = ParamsParser::get();
	params.readFile(getBaseDir() + "\\params.txt");

	std::shared_ptr<AHRS> navx_p = std::make_shared<AHRS>("/dev/ttyACM0");
	while (!navx_p->IsConnected())
		std::this_thread::sleep_for(delay);

	std::shared_ptr<frc::SimMotorController> left_p = std::make_shared<frc::SimMotorController>(0);
	std::shared_ptr<frc::SimMotorController> right_p = std::make_shared<frc::SimMotorController>(1);
	std::shared_ptr<frc::Encoder> left_enc_p = std::make_shared<frc::Encoder>(0, 1);
	std::shared_ptr<frc::Encoder> right_enc_p = std::make_shared<frc::Encoder>(2, 3);
	m_drivebase_p = std::make_shared<DriveBase>(*this, left_p, right_p, left_enc_p, right_enc_p, navx_p);

	int32_t ticks = static_cast<int32_t>(params.getValue("drivebase:ticks_per_rev") + 0.5);
	double diameter = params.getValue("drivebase:diameter");
	double width = params.getValue("drivebase:width");
	double scrub = params.getValue("drivebase:scrub");

	m_drivebase_p->setPhysicalChar(ticks, diameter, width, scrub);
	addSubsystem(m_drivebase_p);

}

START_ROBOT_CLASS(SimRobot);