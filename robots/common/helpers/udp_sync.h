#pragma once
#include "sync.h"
#include <sys/socket.h>

class udpSync :public sync {
public:
	udpSync();
	void getMessage(const std::vector<std::string>&);
	void sendMessage();
};
