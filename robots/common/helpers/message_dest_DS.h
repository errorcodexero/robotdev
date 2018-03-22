#pragma once
#include "message_logger_dest.h"
#include "DriverStation.h"
#include "llvm/Twine.h"

class messageDestDS : public messageLoggerDest {
public:
	messageDestDS(frc::DriverStation* ds);
	void displayMessage(const messageLogger::messageType &type, uint64_t subs, const std::string &msg);
private:
	frc::DriverStation* m_ds;
};