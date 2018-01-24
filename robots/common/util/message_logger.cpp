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

bool messageLogger::enable(const messageType &type) {
	mEnabledModes.at(type) = true;
	return true;
}


bool messageLogger::append(const std::string &message, const messageType& type) {
	bool r = mEnabledModes.at(type);
	if (r) mMessages.at(type).append(message);
	return r;
}
bool messageLogger::append(double message, const messageType& type) {
	bool r = mEnabledModes.at(type);
	if (r) mMessages.at(type).append(std::to_string(message));
	return r;
}
bool messageLogger::append(int message, const messageType& type) {
	bool r = mEnabledModes.at(type);
	if (r) mMessages.at(type).append(std::to_string(message));
	return r;
}
bool messageLogger::endMessage(std::ostream &f, messageType type, bool addEndl = true) {
	if (addEndl) {
		f << mMessages.at(type) << std::endl;
	}
	else
		f << mMessages.at(type);
	return true;
}