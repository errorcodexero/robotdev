#include "message_dest_file.h"

messageDestFile::messageDestFile() {
	m_strm_p = nullptr;
}

messageDestFile::messageDestFile(const std::string &name, int timeout_limit)
{
	setFile(name);
	m_timeout_limit = timeout_limit;
}

messageDestFile::~messageDestFile()
{
	if (m_strm_p != nullptr)
		delete m_strm_p;
}

void messageDestFile::setTimeout(int timeout_limit) {
	m_timeout_limit = timeout_limit;
}

bool messageDestFile::setFile(const std::string &name)
{
	m_filename = name;
	m_strm_p = new std::ofstream(name);
	if (!m_strm_p->is_open())
	{
		if (!m_ref_established) {
			start = std::chrono::steady_clock::now();
			m_ref_established = true;
		}
		std::cerr << "counld not open log file '" << name << "' for writing" << std::endl;
		m_strm_p = nullptr;
		return false;
	}
	return true;
}

void messageDestFile::displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg)
{
	std::string prefix;
	if (type == messageLogger::messageType::warning) prefix == "WARNING: ";
	if (type == messageLogger::messageType::error) prefix == "ERROR: ";
	std::string appended_msg = prefix + msg;
	if (m_strm_p != nullptr) {
		while (m_msg_q.size() > 0) {
			(*m_strm_p) << (*m_msg_q.begin()) << std::endl;
			m_msg_q.pop_front();
		}
		(*m_strm_p) << (appended_msg) << std::endl;
	}
	else {
		if (setFile(m_filename) && m_enabled) {
			while (m_msg_q.size() > 0) {
				(*m_strm_p) << (*m_msg_q.begin()) << std::endl;
				m_msg_q.pop_front();
			}
			(*m_strm_p) << (appended_msg) << std::endl;
		}
		else {
			m_msg_q.push_back(appended_msg);
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
			if (elapsed < m_timeout_limit) m_enabled = true; else m_enabled = false;
		};
	}
}