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
	logger.enable(messageLogger::messageType::debug);
	logger.enable(messageLogger::messageType::warning);
	logger.enable(messageLogger::messageType::error);
	logger.enable(messageLogger::messageType::info);
	
	assert(logger.isMessageTypeEnabled(messageLogger::messageType::debug));
	assert(logger.isMessageTypeEnabled(messageLogger::messageType::warning));
	assert(logger.isMessageTypeEnabled(messageLogger::messageType::info));
	assert(logger.isMessageTypeEnabled(messageLogger::messageType::error));
	
	logger.startMessage(messageLogger::messageType::info) ;
	logger << "This is an info message" ;
	logger.endMessage() ;

	logger.startMessage(messageLogger::messageType::warning) ;
	logger << "This is a warning message" ;
	logger << ", with two calls" << ", and more calls" ;
	logger.endMessaeg() ;

	logger.startMessage(messageLogger::messageType::info) ;
	logger << "The left motor is at " << 1.0 << " volts" ;
	logger << ", and the right motor is at " << 0.8 << " volts" ;
	logger.endMessage() ;
}

int main() {
	test_logger();
	return 0;
}
#endif // MESSAGE_LOGGER_TEST
