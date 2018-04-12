#include "SerialPort.h"
#include <algorithm>
#include <iostream>

namespace frc
{
	SerialPort::SerialPort(Port port)
	{
		m_port = port;
		m_handle = -1;
		SetReadBufferSize(256);
	}

	SerialPort::SerialPort(int baud, Port port)
	{
		m_port = port;
		m_baud = baud;
		m_handle = -1;
		SetReadBufferSize(256);

		open();
	}

	void SerialPort::Reset()
	{
	}

	int SerialPort::Write(char *buffer, int count)
	{
		write(m_handle, buffer, count);
		return count;
	}

	int SerialPort::GetBytesReceived()
	{
		int ret = m_used;
		int bytes = 0;

		if (::ioctl(m_handle, FIONREAD, &bytes) != -1)
		{
			ret += bytes;
		}

		return ret;
	}

	int SerialPort::Read(char *buffer, int count)
	{
		int bytes;
		int ret = 0;
		bool term = false;

		while (count > 0 && !term)
		{
			if (m_used != m_buffer.size())
			{
				if (::ioctl(m_handle, FIONREAD, &bytes) != -1)
				{
					size_t toread = m_buffer.size() - m_used;

					if (toread > static_cast<size_t>(bytes))
						toread = bytes;
					int actual = ::read(m_handle, &m_buffer[m_used], toread);
					m_used += actual;
				}
			}

			size_t index = 0;
			while (index < m_buffer.size() && count > 0)
			{
				*buffer++ = m_buffer[index];
				count--;
				index++;
				ret++;
				if (m_term_mode && m_buffer[index - 1] == m_term)
				{
					term = true;
					break;
				}
			}

			if (index != 0)
			{
				//
				// remove the first index characters from the buffer
				//
				memmove(&m_buffer[0], &m_buffer[index], m_buffer.size() - index);
				m_used -= index;
			}
		}

		return ret;
	}
}
