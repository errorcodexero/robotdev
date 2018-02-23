#pragma once

#include "UdpSender.h"
#include "UdpReceiver.h"
#include <cstdint>
#include <thread>

namespace frc
{
	class DriverStation
	{
	private:
		const uint16_t ROBOT_IN_PORT = 1110;
		const uint16_t ROBOT_OUT_PORT = 1150;

		const uint8_t cTest = 0x01;
		const uint8_t cEnabled = 0x04;
		const uint8_t cAutonomous = 0x02;
		const uint8_t cTeleoperated = 0x00;
		const uint8_t cFMS_Attached = 0x08;
		const uint8_t cEmergencyStop = 0x80;
		const uint8_t cRequestReboot = 0x08;
		const uint8_t cRequestNormal = 0x80;
		const uint8_t cRequestUnconnected = 0x00;
		const uint8_t cRequestRestartCode = 0x04;
		const uint8_t cFMS_RadioPing = 0x10;
		const uint8_t cFMS_RobotPing = 0x08;
		const uint8_t cFMS_RobotComms = 0x20;
		const uint8_t cFMS_DS_Version = 0x00;
		const uint8_t cTagDate = 0x0f;
		const uint8_t cTagGeneral = 0x01;
		const uint8_t cTagJoystick = 0x0c;
		const uint8_t cTagTimezone = 0x10;
		const uint8_t cRed1 = 0x00;
		const uint8_t cRed2 = 0x01;
		const uint8_t cRed3 = 0x02;
		const uint8_t cBlue1 = 0x03;
		const uint8_t cBlue2 = 0x04;
		const uint8_t cBlue3 = 0x05;
		const uint8_t cRTagCANInfo = 0x0e;
		const uint8_t cRTagCPUInfo = 0x05;
		const uint8_t cRTagRAMInfo = 0x06;
		const uint8_t cRTagDiskInfo = 0x04;
		const uint8_t cRequestTime = 0x01;
		const uint8_t cRobotHasCode = 0x20;


	private:
		DriverStation();

	public:
		virtual ~DriverStation();

		static DriverStation &get()
		{
			if (m_ds_p == nullptr)
				initialize();

			return *m_ds_p;
		}

		static void initialize();

	protected:
		void waitForConnection();

	private:
		void dsRecvCommThread();
		void dsSendCommThread();
		void processBaseDSData(const std::vector<uint8_t> &data, size_t start);
		void processTimeZoneData(const std::vector<uint8_t> &data, size_t start);
		void processJoystickData(const std::vector<uint8_t> &data, size_t start);
		uint16_t encodeVoltage(float v);


	private:
		static DriverStation *m_ds_p;

		xeromisc::UdpBroadcastSender *m_server_out_p;
		xeromisc::UdpBroadcastReceiver *m_server_in_p;
		std::thread m_ds_recv_thread;
		std::thread m_ds_send_thread;
		bool m_running;
		uint16_t m_packet_index;

		//
		// Robot characteristics
		//

		//
		// Battery voltage
		//
		float m_voltage;

		//
		// Robot code
		//
		bool m_robot_code;
	};
}
