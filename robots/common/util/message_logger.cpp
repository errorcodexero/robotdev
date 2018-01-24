#include "message_logger.h"

messageLogger::messageLogger() {
	//Initialize maps
	mEnabledModes.insert(std::pair<messageType, bool>(messageType::debug, false));
	mEnabledModes.insert(std::pair<messageType, bool>(messageType::info, false));
	mEnabledModes.insert(std::pair<messageType, bool>(messageType::warning, false));
	mEnabledModes.insert(std::pair<messageType, bool>(messageType::error, false));


	mMessages.insert(std::pair<messageType, std::string>(messageType::debug, ""));
	mMessages.insert(std::pair<messageType, std::string>(messageType::info, ""));
	mMessages.insert(std::pair<messageType, std::string>(messageType::warning, ""));
	mMessages.insert(std::pair<messageType, std::string>(messageType::error, ""));
}

bool messageLogger::getModeState(const messageType &type) const {
	return mEnabledModes.at(type);
}
std::string messageLogger::getMessage(const messageType &type) const {
	return mMessages.at(type);
}
bool messageLogger::enable(const messageType &type) {
	mEnabledModes.at(type) = true;
	return true;
}
bool messageLogger::disable(const messageType &type) {
	mEnabledModes.at(type) = false;
	return true;
}

bool messageLogger::writeMsg(const std::string &message, const messageType& type, bool overwrite) {
	bool r = mEnabledModes.at(type);
	if (r)
		if (!overwrite) mMessages.at(type).append(message);
		else mMessages.at(type) = message;
	return r;
}
bool messageLogger::writeMsg(double message, const messageType& type, bool overwrite) {
	bool r = mEnabledModes.at(type);
	if (r) 
		if (!overwrite) mMessages.at(type).append(std::to_string(message));
		else mMessages.at(type) = std::to_string(message);
	return r;
}
bool messageLogger::writeMsg(int message, const messageType& type, bool overwrite) {
	bool r = mEnabledModes.at(type);
	if (r)
		if (!overwrite) mMessages.at(type).append(std::to_string(message));
		else mMessages.at(type) = std::to_string(message);
		return r;
	return r;
}
bool messageLogger::streamMsg(std::ostream &f, messageType type, bool clearMsg, bool addEndl = true) {
	if (addEndl) {
		f << mMessages.at(type) << std::endl;
	}
	else
		f << mMessages.at(type);
	if (clearMsg) mMessages.at(type) = "";
	return true;
}


#ifdef MESSAGE_LOGGER_TEST

void test_logger() {
	messageLogger logger;
	//Test enable function
	logger.enable(messageType::debug);
	logger.enable(messageType::warning);
	logger.enable(messageType::error);
	logger.enable(messageType::info);
	
	assert(logger.getModeState(messageType::debug));
	assert(logger.getModeState(messageType::warning));
	assert(logger.getModeState(messageType::info));
	assert(logger.getModeState(messageType::error));
	
	//Test disable function
	logger.disable(messageType::info);
	
	assert(!logger.getModeState(messageType::info));
	assert(logger.writeMsg("Error ", messageType::error, false));
	assert(logger.writeMsg("Debug ", messageType::debug, false));
	assert(logger.writeMsg("Warning ", messageType::warning, false));
	assert(!logger.writeMsg("Info ", messageType::info, false));

	//Test writeMsg
	assert(logger.writeMsg("message: ", messageType::error, false));
	assert(logger.writeMsg("message: ", messageType::debug, false));
	assert(logger.writeMsg("message: ", messageType::warning, false));
	assert(!logger.writeMsg("message: ", messageType::info, false));

	int errorNumber = -7, debugNumber = 12;
	double warningNumber = -0.824;

	assert(logger.writeMsg(errorNumber, messageType::error, false));
	assert(logger.writeMsg(debugNumber, messageType::debug, false));
	assert(logger.writeMsg(warningNumber, messageType::warning, false));
	assert(!logger.writeMsg(-912.5, messageType::info, false));

	assert(logger.getMessage(messageType::error) == "Error message: -7");
	assert(logger.getMessage(messageType::debug) == "Debug message: 12");
	assert(logger.getMessage(messageType::warning) == "Warning message: -0.824000");
	assert(logger.getMessage(messageType::info) == "");

	//Test overwriting
	assert(logger.writeMsg("Overwrited error number: ", messageType::error, true));
	assert(logger.writeMsg("Overwrited debug number: ", messageType::debug, true));
	assert(logger.writeMsg("Overwrited warning number: ", messageType::warning, true));
	assert(!logger.writeMsg("Overwrited info number: ", messageType::info, true));

	assert(logger.writeMsg(errorNumber, messageType::error, false));
	assert(logger.writeMsg(debugNumber, messageType::debug, false));
	assert(logger.writeMsg(warningNumber, messageType::warning, false));
	assert(!logger.writeMsg(-912.5, messageType::info, false));

	assert(logger.getMessage(messageType::error) == "Overwrited error number: -7");
	assert(logger.getMessage(messageType::debug) == "Overwrited debug number: 12");
	assert(logger.getMessage(messageType::warning) == "Overwrited warning number: -0.824000");
	assert(logger.getMessage(messageType::info) == "");


}

int main() {
	test_logger();
	return 0;
}
#endif // MESSAGE_LOGGER_TEST
