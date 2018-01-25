#include "Robot2018.h"
#include "Robot2018Consts.h"
#include "NavXAngleMeasurementDevice.h"
#include "EncoderAngleMeasurementDevice.h"
#include "MessageLogger.h"
#include <ctre/phoenix/MotorControl/CAN/WPI_TalonSRX.h>
#include <Encoder.h>
#include "AHRS.h"
#include "ParamFileReader.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace xerolib;

DriveBase::MotionProfileParams Robot2018::StraightParams;
DriveBase::MotionProfileParams Robot2018::RotationalParams;
DriveBase::PIDConstants Robot2018::StraightPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };
DriveBase::PIDConstants Robot2018::DistancePIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants Robot2018::AngleCorrPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants Robot2018::RotationalPIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
DriveBase::PIDConstants Robot2018::AnglePIDConstants = { 0.0, 0.0, 0.0, 0.0, std::numeric_limits<double>::max() };;
double Robot2018::Diameter;
double Robot2018::DistanceThreshold;
double Robot2018::DistanceSwitchThreshold;
double Robot2018::AngleThreshold;
double Robot2018::AngleSwitchThreshold;
int32_t Robot2018::TicksPerRev;
double Robot2018::ErrorThreshold;

const std::string Robot2018::ParamDiameter("drivebase:diameter");
const std::string Robot2018::ParamStraightThreshold("drivebase:straight:threshold");
const std::string Robot2018::ParamStraightSwitchThreshold("drivebase:straight:switch:threshold");
const std::string Robot2018::ParamAngleThreshold("drivebase:angle:threshold");
const std::string Robot2018::ParamAngleSwitchThreshold("drivebase:angle:switch:threshold");
const std::string Robot2018::ParamErrorThreshold("drivebase:error:threshold");
const std::string Robot2018::ParamPulsesPerRev("drivebase:encoderPulsesPerRev");

const std::string Robot2018::ParamStraightAccel("drivebase:straight:accel");
const std::string Robot2018::ParamStraightDecel("drivebase:straight:decel");
const std::string Robot2018::ParamStraightMaxSpeed("drivebase:straight:maxspeed");
const std::string Robot2018::ParamStraightMaxAge("drivebase:straight:maxage");
const std::string Robot2018::ParamStraightP("drivebase:straight:p");
const std::string Robot2018::ParamStraightI("drivebase:straight:i");
const std::string Robot2018::ParamStraightD("drivebase:straight:d");
const std::string Robot2018::ParamStraightF("drivebase:straight:f");
const std::string Robot2018::ParamStraightIMax("drivebase:straight:imax");
const std::string Robot2018::ParamDistanceP("drivebase:distance:p");
const std::string Robot2018::ParamDistanceI("drivebase:distance:i");
const std::string Robot2018::ParamDistanceD("drivebase:distance:d");
const std::string Robot2018::ParamDistanceF("drivebase:distance:f");
const std::string Robot2018::ParamDistanceIMax("drivebase:distance:imax");
const std::string Robot2018::ParamAngleCorrP("drivebase:angleCorrection:p");
const std::string Robot2018::ParamAngleCorrI("drivebase:angleCorrection:i");
const std::string Robot2018::ParamAngleCorrD("drivebase:angleCorrection:d");
const std::string Robot2018::ParamAngleCorrF("drivebase:angleCorrection:f");
const std::string Robot2018::ParamAngleCorrIMax("drivebase:angleCorrection:imax");

const std::string Robot2018::ParamAngleAccel("drivebase:angle:accel");
const std::string Robot2018::ParamAngleDecel("drivebase:angle:decel");
const std::string Robot2018::ParamAngleMaxSpeed("drivebase:angle:maxspeed");
const std::string Robot2018::ParamAngleMaxAge("drivebase:angle:maxage");
const std::string Robot2018::ParamRotationalP("drivebase:rotational:p");
const std::string Robot2018::ParamRotationalI("drivebase:rotational:i");
const std::string Robot2018::ParamRotationalD("drivebase:rotational:d");
const std::string Robot2018::ParamRotationalF("drivebase:rotational:f");
const std::string Robot2018::ParamRotationalIMax("drivebase:rotational:imax");
const std::string Robot2018::ParamAngleP("drivebase:angle:p");
const std::string Robot2018::ParamAngleI("drivebase:angle:i");
const std::string Robot2018::ParamAngleD("drivebase:angle:d");
const std::string Robot2018::ParamAngleF("drivebase:angle:f");
const std::string Robot2018::ParamAngleIMax("drivebase:angle:imax");


Robot2018::Robot2018()
{
}

Robot2018::~Robot2018()
{
}

void Robot2018::extractParams()
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

void Robot2018::dumpParams()
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

void Robot2018::RobotInit()
{
    std::list<std::shared_ptr<frc::SpeedController>> left_motors ;
    std::list<std::shared_ptr<frc::SpeedController>> right_motors ;
    
    std::list<int> left_can_addrs = { 1, 2} ;
    std::list<int> right_can_addrs = { 3, 4 } ;

    frc::Wait(2.0) ;
    
    setupLoggers("robot.log", "robot.csv");
    setupConsoleLogger();
    setupSmartDashboardLogger() ;
    // setupTcpLogger("10.14.25.25", 8888);
    // setupUdpLogger(8888) ;
    
    // readParams();
    extractParams();
    dumpParams();
    
    m_drivebase_p = std::make_shared<DriveBase>(*this);
    
    for(int addr: left_can_addrs)
    {
		std::shared_ptr<frc::SpeedController> motor_p = std::make_shared<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>(addr) ;
		left_motors.push_back(motor_p) ;
    }
    
    for(int addr: right_can_addrs)
    {
		std::shared_ptr<frc::SpeedController> motor_p = std::make_shared<ctre::phoenix::motorcontrol::can::WPI_TalonSRX>(addr) ;
		right_motors.push_back(motor_p) ;
    }
    
    m_drivebase_p->setMotors(left_motors, right_motors);
    
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
}


START_ROBOT_CLASS(Robot2018);
