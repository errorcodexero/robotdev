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
		m_auto_mode = false;
		m_test_mode = false;
		m_restart_robotcode = false;
		m_reboot_pi = false;

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
		uint8_t control = data[start + 3];
		uint8_t request = data[start + 4];
		uint8_t station = data[start + 5];

		if ((control & 0x03) == cTest)
		{
			m_test_mode = true;
			m_auto_mode = false;
		}
		else if ((control & 0x03) == cAutonomous)
		{
			m_test_mode = false;
			m_auto_mode = true;
		}
		else
		{
			m_test_mode = false;
			m_auto_mode = false;
		}

		if ((control & cEnabled) == cEnabled)
			m_enabled = true;
		else
			m_enabled = false;

		if (request == cRequestRestartCode)
			m_restart_robotcode = true;
		else if (request == cRequestReboot)
			m_reboot_pi = true;

		switch (station)
		{
		case cRed1:
			m_alliance = Alliance::kRed;
			m_location = 1;
			break;
		case cRed2:
			m_alliance = Alliance::kRed;
			m_location = 2;
			break;
		case cRed3:
			m_alliance = Alliance::kRed;
			m_location = 3;
			break;
		case cBlue1:
			m_alliance = Alliance::kBlue;
			m_location = 1;
			break;
		case cBlue2:
			m_alliance = Alliance::kBlue;
			m_location = 2;
			break;
		case cBlue3:
			m_alliance = Alliance::kBlue;
			m_location = 3;
			break;
		default:
			m_alliance = Alliance::kRed;
			m_location = 1;
			break;
		}
	}

	void DriverStation::processTimeData(const std::vector<uint8_t> &data, size_t start)
	{
	}

	void DriverStation::processTimeZoneData(const std::vector<uint8_t> &data, size_t start)
	{
	}

	void DriverStation::processJoystickData(int index, const std::vector<uint8_t> &data, size_t start)
	{
		start += 2;

		uint8_t axiscount = data[start++];
		setJoystickAxisCount(index, axiscount);
		for (size_t i = 0; i < axiscount; i++)
			setJoystickAxisValue(index, i, byteToFloat(data[start++]);

		uint8_t buttoncount = data[start++];
		setJoystickButtonCount(index, buttoncount);
		uint16_t buttons = data[start++] << 8;
		buttons |= data[start++];
		setJoystickButtons(uint16_t);

		uint8_t povcount = data[start++];
		setPOVCount(povcount);
		for (size_t i = 0; i < povcount; i++)
		{
			uint16_t value = data[start++] << 8;
			value |= data[start];
			setJoystickPOVValue(index, i, value);
		}
	}

	void DriverStation::dsRecvCommThread()
	{
		std::vector<uint8_t> data(128);
		int count;
		int index;

		while (m_running)
		{
			size_t sofar = 0;
			count = m_server_in_p->receive(data);
			if (count < 6)
				continue;

			index = 0;

			processBaseDSData(data, sofar);
			sofar += 6;

			while (sofar < count - 1)
			{
				uint8_t len = data[sofar];
				uint8_t tag = data[sofar + 1];

				if (tag == cTagJoystick)
				{
					processJoystickData(index++, data, sofar);
					sofar += len;
				}
				else if (tag == cTagDate)
				{
					processTimeData(data, sofar);
					sofar += len;
				}
				else if (tag == cTagTimezone)
				{
					processTimeZoneData(data, sofar);
					sofar += len;
				}
				else
				{
					//
					// Unknown tag
					//
					sofar += len;
				}
			}
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
