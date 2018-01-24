#pragma once
#include <iostream>
#include <string>
#include <map>
#include <cassert>


enum class messageType { debug, info, warning, error };

class messageLogger {
public:
	messageLogger();
	bool getModeState(const messageType&) const;
	std::string getMessage(const messageType&) const;
	bool enable(const messageType&);					//Enable a message type. Only enabled message types can be streamed
	bool disable(const messageType&);
	bool writeMsg(const std::string &message, const messageType&, bool overwrite);		//Append to the message of the selected type
	bool writeMsg(double message, const messageType&, bool overwrite);					//Append to the message of the selected type
	bool writeMsg(int message, const messageType&, bool overwrite);						//Append to the message of the selected type
	bool streamMsg(std::ostream &f, messageType type, bool addEndln, bool clearMsg); //Stream message of a type
private:
	std::map<messageType, bool> mEnabledModes;
	std::map <messageType, std::string> mMessages;
	messageType currentMode;

};