#include "stdafx.h"
#include "paramsParser.h"


paramsInput::paramsInput() {
}

bool paramsInput::readFile(const std::string &filename) {
	this->filename = filename;
	f.open(filename);
	if (!f.is_open()) { return false; }
	else {
		std::string buffer;
		std::pair<std::string, double> bufferPair;
		while (!f.eof()) {
			std::getline(f, buffer);
			unsigned int commentStart = buffer.find('#');
			if (commentStart != buffer.npos) {
				buffer = buffer.substr(0, commentStart);
			}
			if (buffer.length() > 0) {
				unsigned int i = 0;
				std::string Key, valueString;
				double value;
				while (i < buffer.length() && (!isspace(buffer[i])))
					Key += buffer[i++];

				while (i < buffer.length() && (isspace(buffer[i])))
					i++;

				while (i < buffer.length() && (!isspace(buffer[i])))
					valueString += buffer[i++];

				value = std::stod(valueString);
				paramsMap.insert(std::pair<std::string, double>(Key, value));
			}
		}
		return true;
	}
}

bool paramsInput::printMap(const std::string &filename) {
	std::ofstream fo;
	fo.open(filename);
	if (!fo.is_open()) return false;
	else {
		std::map<std::string, double>::iterator it = paramsMap.begin();
		for (; it != paramsMap.end(); it++) {
			fo << it->first << ' ' << it->second << std::endl;
		}
	}
	fo.close();
}
bool paramsInput::hasParam(const std::string &paramName) {
	bool found = false;
	if (paramsMap.find(paramName) != paramsMap.end()) found = true;
	return found;
}
double paramsInput::getValue(const std::string &paramName, double defaultValue) {
	std::map<std::string, double>::iterator pos = paramsMap.find(paramName);
	if (paramsMap.find(paramName) != paramsMap.end()) return pos->second;
}