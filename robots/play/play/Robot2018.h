#pragma once

#include "XeroRobotBase.h"
#include "DriveBase.h"
#include "Robot2018Autonomous.h"
#include <map>
#include <string>

class Robot2018 : public xerolib::XeroRobotBase
{
public:
	Robot2018();
	~Robot2018();

	virtual void RobotInit();

protected:
	/// @brief this method is implemented by a derived class and creates the autonomous controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createAutonomousController()
	{
		std::shared_ptr<Robot2018Autonomous> ctrl_p = std::make_shared<Robot2018Autonomous>(*this);
		return ctrl_p;
	}

	/// @brief this method is implemented by a derived class and creates the operator controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createOperatorControlController()
	{
		return nullptr;
	}

	/// @brief this method is implemented by a derived class and creates the test controller
	virtual std::shared_ptr<xerolib::RobotControllerBase> createTestController()
	{
		return nullptr;
	}

private:
	static const std::string ParamDiameter;
	static const std::string ParamStraightThreshold;
	static const std::string ParamAngleThreshold;
	static const std::string ParamErrorThreshold;
	static const std::string ParamPulsesPerRev;

	static const std::string ParamStraightAccel;
	static const std::string ParamStraightDecel;
	static const std::string ParamStraightMaxSpeed;
	static const std::string ParamStraightMaxAge;
	static const std::string ParamStraightP;
	static const std::string ParamStraightI;
	static const std::string ParamStraightD;
	static const std::string ParamStraightF;
	static const std::string ParamStraightIMax;
	static const std::string ParamDistanceP;
	static const std::string ParamDistanceI;
	static const std::string ParamDistanceD;
	static const std::string ParamDistanceF;
	static const std::string ParamDistanceIMax;
	static const std::string ParamAngleCorrP;
	static const std::string ParamAngleCorrI;
	static const std::string ParamAngleCorrD;
	static const std::string ParamAngleCorrF;
	static const std::string ParamAngleCorrIMax;

	static const std::string ParamAngleAccel;
	static const std::string ParamAngleDecel;
	static const std::string ParamAngleMaxSpeed;
	static const std::string ParamAngleMaxAge;
	static const std::string ParamRotationalP;
	static const std::string ParamRotationalI;
	static const std::string ParamRotationalD;
	static const std::string ParamRotationalF;
	static const std::string ParamRotationalIMax;
	static const std::string ParamAngleP;
	static const std::string ParamAngleI;
	static const std::string ParamAngleD;
	static const std::string ParamAngleF;
	static const std::string ParamAngleIMax;

	static xerolib::DriveBase::MotionProfileParams StraightParams;
	static xerolib::DriveBase::MotionProfileParams RotationalParams;
	static xerolib::DriveBase::PIDConstants StraightPIDConstants;
	static xerolib::DriveBase::PIDConstants DistancePIDConstants;
	static xerolib::DriveBase::PIDConstants AngleCorrPIDConstants;
	static xerolib::DriveBase::PIDConstants RotationalPIDConstants;
	static xerolib::DriveBase::PIDConstants AnglePIDConstants;

	static double Diameter;
	static double DistanceThreshold;
	static double AngleThreshold;
	static double ErrorThreshold;
	static int32_t TicksPerRev;

private:
	void extractParams();
	void dumpParams();

private:
	//
	// The drive base for this robot
	//
	std::shared_ptr<xerolib::DriveBase> m_drivebase_p;

	//
	// The eyes for this robot
	//
	std::shared_ptr<ServoSubsystem> m_eyes_p;

};
