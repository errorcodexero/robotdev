#include "DriverStation.h"
#include <stdexcept>
#include <thread>
#include <iomanip>
#include <cmath>

namespace frc
{
	DriverStation *DriverStation::m_ds_p = nullptr;

	DriverStation::DriverStation()
	{
		m_voltage = 11.2f;
		m_robot_code = true;

		m_packet_index = 0;
		m_running = false;
		waitForConnection();
	}

	DriverStation::~DriverStation()
	{
	}

	void DriverStation::initialize()
	{
		if (m_ds_p == nullptr)
			m_ds_p = new DriverStation();
	}

	void DriverStation::waitForConnection()
	{
		m_server_in_p = new xeromisc::UdpBroadcastReceiver();
		if (!m_server_in_p->open(ROBOT_IN_PORT))
		{
			std::string err("could not connect to driver station - in port");
			std::runtime_error ex(err.c_str());
			throw ex;
		}

		m_server_out_p = new xeromisc::UdpBroadcastSender();
		if (!m_server_out_p->open(ROBOT_OUT_PORT))
		{
			std::string err("could not connect to driver station - out port");
			std::runtime_error ex(err.c_str());
			throw ex;
		}

		std::cout << "DriverStation connection established" << std::endl;

		m_running = true;
		m_ds_recv_thread = std::thread(&DriverStation::dsRecvCommThread, this);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		m_ds_send_thread = std::thread(&DriverStation::dsSendCommThread, this);
	}

	void DriverStation::processBaseDSData(const std::vector<uint8_t> &data, size_t start)
	{
	}

	void DriverStation::processTimeZoneData(const std::vector<uint8_t> &data, size_t start)
	{
	}

	void DriverStation::processJoystickData(const std::vector<uint8_t> &data, size_t start)
	{
	}

	void DriverStation::dsRecvCommThread()
	{
		std::vector<uint8_t> data(128);
		int count;

		while (m_running)
		{
			count = m_server_in_p->receive(data);
			if (count < 6)
				continue;

			processBaseDSData(data, 0);
		}
	}

	uint16_t DriverStation::encodeVoltage(float voltage)
	{
		uint16_t ret;
		float intpart, fracpart;

		fracpart = std::modf(voltage, &intpart);

		ret = static_cast<uint16_t>(static_cast<uint8_t>(intpart) << 8);
		ret = static_cast<uint16_t>(ret | static_cast<uint8_t>(fracpart * 100));

		return ret;
	}

	void DriverStation::dsSendCommThread()
	{
		std::chrono::milliseconds sleeptime(100);
		std::vector<uint8_t> data(7);
		size_t count;

		while (m_running)
		{
			uint16_t voltage = encodeVoltage(m_voltage);
			uint8_t control = 0;
			uint8_t rstatus = 0;
			uint8_t request = 0;

			if (m_robot_code)
				rstatus |= cRobotHasCode;

			count = 7;
			data[0] = static_cast<uint8_t>(m_packet_index >> 8);
			data[1] = static_cast<uint8_t>(m_packet_index);
			data[2] = cTagGeneral;

			data[3] = control;
			data[4] = rstatus;
			data[5] = static_cast<uint8_t>(voltage >> 8);
			data[6] = static_cast<uint8_t>(voltage);
			data[7] = request;

			m_server_out_p->send(data, 0, count);
			std::this_thread::sleep_for(sleeptime);
		}
	}
}
