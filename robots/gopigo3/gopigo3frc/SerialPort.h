#pragma once

namespace frc
{
	class SerialPort
	{
	public:
		enum class Port
		{
			Port_0 = 0,
			Port_1 = 1,
			kMXP,
			kOnboard,
			kUSB
		};

	public:
		SerialPort(Port port)
		{
			m_port = port;
		}

		SerialPort(int baud, Port port)
		{
			m_port = port;
		}

		virtual ~SerialPort()
		{
		}

		void Flush()
		{
		}

		int GetBytesReceived()
		{
			return 0;
		}

		int Read(char *byffer, int count)
		{
			return count;
		}

		int Write(char *buffer, int count)
		{
			return count;
		}

		void SetReadBufferSize(int size)
		{
		}

		void SetTimeout(double timeout)
		{
		}

		void EnableTermination(char terminator = '\n')
		{
		}

		void Reset()
		{
		}

	private:
		Port m_port;
	};
}

