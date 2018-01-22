#pragma once

#include <string>
#include <list>
#include <memory>

namespace xerolib
{
    class MessageDestination;

    class MessageLogger
    {
    public:
	enum class MessageType
	{
	    Error,
	    Warning,
	    Info,
	    Debug,
	};

	enum class Token
	{
	    EndOfMessage,
	};

    public:
	MessageLogger();
	~MessageLogger();

	void addDestination(std::shared_ptr<MessageDestination> dest_p);
	void addDestination(const char *filename_p);
	void addDestination(const std::string &filename);
	void addStandardOutputDestination();

	MessageLogger &operator<<(MessageType type);
	MessageLogger &operator<<(Token token);

	MessageLogger &operator<<(const char *value_p);
	MessageLogger &operator<<(const std::string &value);
	MessageLogger &operator<<(double value);
	MessageLogger &operator<<(int8_t value);
	MessageLogger &operator<<(int16_t value);
	MessageLogger &operator<<(int32_t value);
	MessageLogger &operator<<(int64_t value);
	MessageLogger &operator<<(uint8_t value);
	MessageLogger &operator<<(uint16_t value);
	MessageLogger &operator<<(uint32_t value);
	MessageLogger &operator<<(uint64_t value);

    protected:
	void SendMessage(MessageType type, const std::string &message);

    private:
	// If true, we are in the middle of a message
	bool m_in_message;

	// If in the middle of a message, the type of message
	MessageType m_type;

	// If in the middle of a message, the text of the message so far
	std::string m_message;

	// The types of messages that actual get sent to the list of destinations
	std::list<MessageType> m_send_types;

	// The list of destinations
	std::list<std::shared_ptr<MessageDestination>> m_destinations;
    };
}

