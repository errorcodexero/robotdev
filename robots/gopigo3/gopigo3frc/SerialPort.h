#pragma once

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <vector>
#include <cstdint>
#include <string.h>
#include <iostream>

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
		SerialPort(Port port);
		SerialPort(int baud, Port port);

		virtual ~SerialPort()
		{
			if (m_handle != -1)
				::close(m_handle);
		}

		void Flush()
		{
		}

		int GetBytesReceived();

		int Read(char *buffer, int count);
		int Write(char *buffer, int count);

		void SetReadBufferSize(int size)
		{
			m_buffer.resize(size);
			m_used = 0;
		}

		void SetTimeout(double timeout)
		{
			m_timeout = timeout;
		}

		void EnableTermination(char terminator = '\n')
		{
			m_term_mode = true;
			m_term = terminator;
		}

		void Reset();

	private:
		void GetData();

	private:

		const char *PortToName(Port p)
		{
			const char *name_p = nullptr;

			switch (m_port)
			{
			case Port::Port_0:
				name_p = "/dev/ttyACM0";
				break;

			default:
				name_p = nullptr;
				break;
			}

			return name_p;
		}

		void open()
		{
			const char *name_p = PortToName(m_port);

			m_handle = ::open(name_p, O_RDWR | O_NOCTTY | O_NDELAY, 0);
			if (m_handle != -1)
			{
				::fcntl(m_handle, F_SETFL, FNDELAY);

				struct termios options;
				if (tcgetattr(m_handle, &options) == -1)
				{
					std::cout << "SerialPort: tcgetattr failed, errno is " << errno << std::endl;
					::close(m_handle);
					m_handle = -1;
					return;
				}
				cfsetispeed(&options, B57600);
				cfsetospeed(&options, B57600);

				options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
				options.c_oflag = 0;
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG | IEXTEN);

				options.c_cflag |= (CLOCAL | CREAD);
				options.c_cflag &= ~CSTOPB;						// One stop bit

				options.c_cflag &= ~(CSIZE | PARENB);
				options.c_cflag |= CS8;							// Eight bits, no parity

				if (tcsetattr(m_handle, TCSANOW, &options) == -1)
				{
					std::cout << "SerialPort: tcsetattr failed, errno is " << errno << std::endl;
					::close(m_handle);
					m_handle = -1;
					return;
				}
			}
		}

	private:
		Port m_port;
		int m_baud;
		int m_handle;
		bool m_term_mode;
		char m_term;
		size_t m_used;
		double m_timeout;
		std::vector<uint8_t> m_buffer;
	};
}

