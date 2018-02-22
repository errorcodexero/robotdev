#include "DriverStation.h"
#include <stdexcept>
#include <thread>
#include <iomanip>

namespace frc
{
	DriverStation *DriverStation::m_ds_p = nullptr;

	DriverStation::DriverStation()
	{
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

	void DriverStation::dsRecvCommThread()
	{
		std::vector<uint8_t> data(128);
		int count;

		std::cout << "Running DS receive thread" << std::endl;
		while (m_running)
		{
			count = m_server_in_p->receive(data);
			if (count == -1)
				std::cout << "Error reading UDP packet" << std::endl;
			else
			{
				std::cout << "packet: " << count << ":";
				std::cout << std::hex << std::setfill('0');

				for (int i = 0; i < count; i++)
					std::cout << " " << std::setw(2) << data[i];

				std::cout << std::dec << std::setfill(' ');
				std::cout << std::endl;
			}
		}
	}

	void DriverStation::dsSendCommThread()
	{
		std::chrono::milliseconds sleeptime(10);
		std::cout << "Running DS send thread" << std::endl;
		while (m_running)
		{
			std::this_thread::sleep_for(sleeptime);
		}
	}
}
