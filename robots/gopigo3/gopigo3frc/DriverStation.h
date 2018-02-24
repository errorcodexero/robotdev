#pragma once

#include "UdpSender.h"
#include "UdpReceiver.h"
#include <cstdint>
#include <thread>
#include <vector>

namespace frc
{
	class DriverStation
	{
	private:
		static const uint16_t ROBOT_IN_PORT = 1110;
		static const uint16_t ROBOT_OUT_PORT = 1150;

		static const uint8_t cTest = 0x01;
		static const uint8_t cEnabled = 0x04;
		static const uint8_t cAutonomous = 0x02;
		static const uint8_t cTeleoperated = 0x00;
		static const uint8_t cFMS_Attached = 0x08;
		static const uint8_t cEmergencyStop = 0x80;
		static const uint8_t cRequestReboot = 0x08;
		static const uint8_t cRequestNormal = 0x80;
		static const uint8_t cRequestUnconnected = 0x00;
		static const uint8_t cRequestRestartCode = 0x04;
		static const uint8_t cFMS_RadioPing = 0x10;
		static const uint8_t cFMS_RobotPing = 0x08;
		static const uint8_t cFMS_RobotComms = 0x20;
		static const uint8_t cFMS_DS_Version = 0x00;
		static const uint8_t cTagDate = 0x0f;
		static const uint8_t cTagGeneral = 0x01;
		static const uint8_t cTagJoystick = 0x0c;
		static const uint8_t cTagTimezone = 0x10;
		static const uint8_t cRed1 = 0x00;
		static const uint8_t cRed2 = 0x01;
		static const uint8_t cRed3 = 0x02;
		static const uint8_t cBlue1 = 0x03;
		static const uint8_t cBlue2 = 0x04;
		static const uint8_t cBlue3 = 0x05;
		static const uint8_t cRTagCANInfo = 0x0e;
		static const uint8_t cRTagCPUInfo = 0x05;
		static const uint8_t cRTagRAMInfo = 0x06;
		static const uint8_t cRTagDiskInfo = 0x04;
		static const uint8_t cRequestTime = 0x01;
		static const uint8_t cRobotHasCode = 0x20;

	private:
		class JoystickState
		{
		public:
			JoystickState()
			{
				m_buttons = 0;
			}

			~JoystickState()
			{
			}



		private:
			uint16_t m_buttons;
			std::vector<float> m_axis;
			std::vector<uint16_t> m_pov;
		};

	private:
		DriverStation();

	public:
		enum class Alliance
		{
			kRed,
			kBlue,
		};
	public:
		virtual ~DriverStation();

		static DriverStation &get()
		{
			if (m_ds_p == nullptr)
				initialize();

			return *m_ds_p;
		}

		bool IsEnabled() const
		{
			return m_enabled;
		}

		bool IsDisabled() const
		{
			return !m_enabled;
		}

		bool IsAutonomous() const
		{
			return m_auto_mode;
		}

		bool IsOperatorControl() const
		{
			return !m_auto_mode && !m_test_mode;
		}

		bool IsTest() const
		{
			return m_test_mode;
		}

		Alliance GetAlliance() const
		{
			return m_alliance;
		}

		int GetLocation() const
		{
			return m_location;
		}

		static void initialize();

	protected:
		void waitForConnection();

	private:
		void dsRecvCommThread();
		void dsSendCommThread();
		void processBaseDSData(const std::vector<uint8_t> &data, size_t start);
		void processTimeData(const std::vector<uint8_t> &data, size_t start);
		void processTimeZoneData(const std::vector<uint8_t> &data, size_t start);
		void processJoystickData(int index, const std::vector<uint8_t> &data, size_t start);
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
		bool m_test_mode;
		bool m_auto_mode;
		bool m_enabled;

		bool m_restart_robotcode;
		bool m_reboot_pi;

		Alliance m_alliance;
		int m_location;

		std::vector<JoystickState> m_joysticks;

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
