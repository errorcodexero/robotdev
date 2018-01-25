#include "MessageLogger.h"
#include "MessageStreamDestination.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace xerolib
{

    MessageLogger::MessageLogger()
    {
	m_send_types.push_back(MessageType::Error);
	m_send_types.push_back(MessageType::Warning);
	m_send_types.push_back(MessageType::Info);

#ifdef _DEBUG
	m_send_types.push_back(MessageType::Debug);
#endif	
    }

    MessageLogger::~MessageLogger()
    {
    }

    void MessageLogger::addDestination(std::shared_ptr<MessageDestination> dest_p)
    {
	m_destinations.push_back(dest_p);
    }

    void MessageLogger::addDestination(const char *filename_p)
    {
	auto dest_p = std::make_shared<MessageStreamDestination>(filename_p);
	m_destinations.push_back(dest_p);
    }

    void MessageLogger::addDestination(const std::string &filename)
    {
	auto dest_p = std::make_shared<MessageStreamDestination>(filename.c_str());
	m_destinations.push_back(dest_p);
    }

    void MessageLogger::addStandardOutputDestination()
    {
	auto dest_p = std::make_shared<MessageStreamDestination>(std::cout);
	m_destinations.push_back(dest_p);
    }

    void MessageLogger::SendMessage(MessageType type, const std::string &text)
    {
	if (std::find(m_send_types.begin(), m_send_types.end(), type) != m_send_types.end())
	{
	    //
	    // Send the message to the stream given
	    //
	    for (auto dest_p : m_destinations)
		dest_p->SendMessage(type, text);
	}
    }

    MessageLogger &MessageLogger::operator<<(MessageType type)
    {
	if (m_in_message)
	{
	    std::runtime_error err("cannot start a new message while an existing message is still active");
	    throw err;
	}

	m_in_message = true;
	m_type = type;
	m_message.clear();
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(Token token)
    {
	if (token == Token::EndOfMessage)
	{
	    SendMessage(m_type, m_message);
	    m_in_message = false;
	}
	else
	{
	    std::runtime_error err("invalid Token value in method operator<<(Token)");
	    throw err;
	}
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(const char *value_p)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += value_p;
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(const std::string &value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += value;
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(double value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}

	if (value > 100000 || value < -100000)
	{
	    std::stringstream strm;

	    strm << value;
	    m_message += strm.str();
	}
	else
	{
	    m_message += std::to_string(value);
	}
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(int8_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(int16_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(int32_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(int64_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(uint8_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(uint16_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(uint32_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }

    MessageLogger &MessageLogger::operator<<(uint64_t value)
    {
	if (!m_in_message)
	{
	    std::runtime_error err("cannot send data until a message is started with a message type");
	    throw err;
	}
	m_message += std::to_string(value);
	return *this;
    }
}
