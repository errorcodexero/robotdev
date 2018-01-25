#include "message_logger.h"

messageLogger::messageLogger() {
	//Initialize maps
	
}

void messageLogger::enable(const messageType &type) {
	mEnabledModes.push_back(type);
}
void messageLogger::disable(const messageType &type) {
	mEnabledModes.erase(std::find(mEnabledModes.begin(), mEnabledModes.end(), type));
}
bool messageLogger::isMessageTypeEnabled(const messageType &type) {
	auto it = mEnabledModes.begin();
	for (; it != mEnabledModes.end(); it++) {
		if ((*it) == type) return true;
	}
	return false;
}
void messageLogger::startMessage(const messageType &type) {
	assert(!mInMessage);
	mCurrentType = type;
	mInMessage = true;
}
void messageLogger::endMessage() {
	assert(mInMessage);
}

messageLogger& messageLogger::operator<<(const std::string &value) {
	assert(mInMessage);
	if (isMessageTypeEnabled(mCurrentType))
	m_current_message.append(value);
	return *this;
}

messageLogger& messageLogger::operator<<(const char *value_p) {
	assert(mInMessage);
	if (isMessageTypeEnabled(mCurrentType))
		m_current_message.append(std::to_string(*value_p));
	return *this;
}

messageLogger& messageLogger::operator<<(int value) {
	assert(mInMessage);
	if (isMessageTypeEnabled(mCurrentType))
		m_current_message.append(std::to_string(value));
	return *this;
}

messageLogger& messageLogger::operator<<(double value) {
	assert(mInMessage);
	if (isMessageTypeEnabled(mCurrentType))
		m_current_message.append(std::to_string(value));
	return *this;
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
