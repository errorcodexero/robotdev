#include "message_logger.h"
#include <sstream>
#include <iostream>

messageLogger *messageLogger::mTheOneLogger = nullptr ;

messageLogger::messageLogger()
{
    //Initialize maps
    m_current_message = "";
}

messageLogger &messageLogger::get()
{
    if (mTheOneLogger == nullptr)
	mTheOneLogger = new messageLogger() ;

    return *mTheOneLogger ;
}

void messageLogger::enable(const messageType &type)
{
    mEnabledModes.push_back(type);
}

void messageLogger::disable(const messageType &type)
{
    //
    // Vi: I changed this to be more robust.  If someone calls disable with
    //     a type that is not enabled, the previous implementation
    //     would throw an exception when you tried to call erase on an
    //     iterator that was equal to mEnabledModes.end()
    //
    auto it = std::find(mEnabledModes.begin(), mEnabledModes.end(), type) ;
    if (it != mEnabledModes.end())
	mEnabledModes.erase(it) ;
}

bool messageLogger::isMessageTypeEnabled(const messageType &type)
{
    for (auto it = mEnabledModes.begin() ; it != mEnabledModes.end(); it++) {
	if ((*it) == type)
	    return true;
    }
    return false;
}

void messageLogger::startMessage(const messageType &type)
{
    assert(!mInMessage);
    mCurrentType = type;
    mInMessage = true;
}

void messageLogger::endMessage()
{
    assert(mInMessage);
    mInMessage = false;
    std::cout << m_current_message ;
    m_current_message = "";
}

messageLogger& messageLogger::operator<<(const std::string &value)
{
    assert(mInMessage);
    if (isMessageTypeEnabled(mCurrentType))
	m_current_message.append(value);
    return *this;
}

messageLogger& messageLogger::operator<<(const char *value_p)
{
    assert(mInMessage);
    if (isMessageTypeEnabled(mCurrentType))
	m_current_message.append(std::string(value_p));
    return *this;
}

messageLogger& messageLogger::operator<<(int value)
{
    assert(mInMessage);
    if (isMessageTypeEnabled(mCurrentType))
	m_current_message.append(std::to_string(value));
    return *this;
}

messageLogger& messageLogger::operator<<(double value)
{
    assert(mInMessage);
    if (isMessageTypeEnabled(mCurrentType))
	m_current_message.append(std::to_string(value));
    return *this;
}
#ifdef MESSAGE_LOGGER_TEST

void test_logger()
{
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

    logger.disable(messageLogger::messageType::info) ;
    logger.disable(messageLogger::messageType::info) ;

    std::stringstream ss;

    logger.startMessage(messageLogger::messageType::info);
    logger << "This is an info message";
    logger.endMessage(ss);
    assert(ss.str() == "This is an info message");


    ss = std::stringstream();;

    logger.startMessage(messageLogger::messageType::warning);
    logger << "This is a warning message";
    logger << ", with two calls" << ", and more calls";
    logger.endMessage(ss);
    assert(ss.str() == "This is a warning message, with two calls, and more calls");

    ss = std::stringstream();;
    logger.startMessage(messageLogger::messageType::info);
    logger << "The left motor is at " << 1.0 << " volts";
    logger << ", and the right motor is at " << 0.8 << " volts";
    logger.endMessage(ss);
    assert(ss.str() == "The left motor is at 1.000000 volts, and the right motor is at 0.800000 volts");
}

int main()
{
    test_logger();
    return 0;
}
#endif // MESSAGE_LOGGER_TEST
