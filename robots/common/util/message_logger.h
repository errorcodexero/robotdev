#pragma once
#include <iostream>
#include <string>
#include <map>


enum class messageType { debug, info, warning, error };

class messageLogger {
public:
	messageLogger();
	bool enable(const messageType&);					//Enable a message type. Only enabled message types can be streamed
	bool append(const std::string &message, const messageType&);		//Append to the message of the selected type
	bool append(double message, const messageType&);					//Append to the message of the selected type
	bool append(int message, const messageType&);						//Append to the message of the selected type
	bool endMessage(std::ostream &f, messageType type, bool addEndln); //Stream message of a type
private:
	std::map<messageType, bool> mEnabledModes;
	std::map <messageType, std::string> mMessages;
	messageType currentMode;

};