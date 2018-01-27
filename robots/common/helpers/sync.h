#pragma once
#include <iostream>
#include <string>
#include <vector>


class sync {
public:
	sync();
	virtual void getMessage(const std::string&);
	virtual void getMessage(const std::vector<std::string>&);
	virtual void sendMessage();
};
