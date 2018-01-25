#pragma once
#include <iostream>
#include <string>
#include <list>
#include <cassert>


class messageLogger {
public:
	
	/// \brief the type of a message being logged
	enum class messageType {
		debug,			///< the message is a debug message
		info,			///< the message is informational
		warning,		///< the message is a warning
		error			///< the message is an error message
	};
public:
	/// \brief create a new message logger object
	messageLogger();

	/// \brief returns true if a given message type is active
	/// \param the type of message to check for active
	/// \returns true if the message type is active, otherwise false
	bool isMessageTypeEnabled(const messageType &type) ;

	/// \brief enable a given message type
	/// \param type the type of message to enable
	void enable(const messageType &type);
	
	/// \brief disable a given message type
	/// \param type the type of message to disable
	void disable(const messageType &type);

	/// \brief start a new message
	/// \param type the type of message to start
	void startMessage(const messageType &type) ;

	/// \brief end the current message
	void endMessage() ;

	/// \brief operator overload the output a string value
	/// \param value_p the string to output
	/// \returns a copy of the message logger
	messageLogger &operator<<(const char *value_p)  ;
	
	/// \brief operator overload the output a string value
	/// \param value the string to output
	/// \returns a copy of the message logger
	messageLogger &operator<<(const std::string &value) ;
	
	/// \brief operator overload the output an integer value
	/// \param value the integer to output
	/// \returns a copy of the message logger
	messageLogger &operator<<(int value) ;
	
	/// \brief operator overload the output a floating point value
	/// \param value the floating point number to output
	/// \returns a copy of the message logger
	messageLogger &operator<<(double value) ;

private:
	// The modes currently enabled
	std::list<messageType> mEnabledModes ;

	// If true, we have seen a startMessage() call but not an
	// endMessage() call
	bool mInMessage ;

	// The current message type
	messageType mCurrentType ;

	// The current message
	std::string m_current_message ;
};
