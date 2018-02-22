#pragma once

#include "UdpBroadcastSender.h"
#include "UdpBroadcastReceiver.h"
#include <cstdint>
#include <thread>

namespace frc
{
	class DriverStation
	{
	private:
		const uint16_t ROBOT_IN_PORT = 1110;
		const uint16_t ROBOT_OUT_PORT = 1150;

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

	private:
		static DriverStation *m_ds_p;

		xeromisc::UdpBroadcastSender *m_server_out_p;
		xeromisc::UdpBroadcastReceiver *m_server_in_p;
		std::thread m_ds_recv_thread;
		std::thread m_ds_send_thread;
		bool m_running;
	};
}
