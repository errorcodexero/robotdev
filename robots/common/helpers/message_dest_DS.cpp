#include "message_dest_DS.h"

messageDestDS::messageDestDS(frc::DriverStation* ds) {
	m_ds = ds;

};

void messageDestDS::displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg) {
	if (type == messageLogger::messageType::error) {
		m_ds->ReportError(msg);
	}
	if (type == messageLogger::messageType::warning) {
		m_ds->ReportWarning(msg);
	}
}