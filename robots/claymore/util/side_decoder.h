#pragma once
#include <string>
 

class sideDecoder {
private:
	std::string sideString;
public:
	sideDecoder(const std::string& sides);
	enum class side { LEFT, RIGHT };
	side nearSwitch();
	side scale();
	side farSwitch();
};