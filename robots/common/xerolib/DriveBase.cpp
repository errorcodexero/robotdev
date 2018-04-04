#include "DriveBase.h"
#include "XeroRobotBase.h"
#include <WPILib.h>
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>

namespace xerolib
{
    DriveBase::DriveBase(XeroRobotBase &robot) : SubsystemBase("drivebase", robot)
    {
		m_left_voltage = 0.0;
		m_right_voltage = 0.0;

		m_max_voltage_change = 1.0 ;
		m_max_voltage = 0.6 ;

		//
		// Create the speedometers
		//
		m_linear_speedometer_p = std::make_shared<Speedometer>(4);
		m_left_linear_speedometer_p = std::make_shared<Speedometer>(4);
		m_right_linear_speedometer_p = std::make_shared<Speedometer>(4);
    }

    DriveBase::~DriveBase()
    {
    }

    double DriveBase::getDistance()
    {
		int32_t left = m_left_encoder_p->Get();
		int32_t right = m_right_encoder_p->Get();
		int32_t avg = (left + right) / 2;
		double rev = (double)avg / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI ;
    }

    double DriveBase::getLeftDistance()
    {
		int32_t left = m_left_encoder_p->Get();
		double rev = (double)left / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
    }

    double DriveBase::getRightDistance()
    {
		int32_t right = m_right_encoder_p->Get();
		double rev = (double)right / (double)m_ticks_per_rev;
		return rev * m_wheel_diameter * PI;
    }

    double DriveBase::getAngle()
    {
		return m_angle_measure_p->getAngle();
    }

    void DriveBase::readInputs()
    {
		xerolib::XeroRobotBase &robot = getRobot();
		DataLogger &data = robot.getDataLogger();
		MessageLogger &logger = robot.getMessageLogger();
		double now = robot.getTime();

		m_linear_speedometer_p->addSample(now, getDistance());
		m_left_linear_speedometer_p->addSample(now, getLeftDistance());
		m_right_linear_speedometer_p->addSample(now, getRightDistance());
    }

    void DriveBase::setOutputs()
    {
		if (m_mode == Mode::Idle)
		{
			//
			// Drivebase is idle, motors should be stopped
			//
			m_left_voltage = 0.0;
			m_right_voltage = 0.0;
		}
		else if (m_mode == Mode::Manual)
		{
			//
			// Leave the voltages where they are
			//
		}
		else if (m_mode == Mode::FollowPath)
		{
			//
			// We are following a path
			//
		}

		//
		// The the physical motor voltages
		//
		setMotors();
    }

    void DriveBase::setMotors()
    {
		for (auto motor_p : m_left_motors)
			motor_p->Set(m_left_voltage);

		for (auto motor_p : m_right_motors)
			motor_p->Set(m_right_voltage);
    }
}
