#pragma once
#include <string>
#include <fstream>
#include <map>


class paramsInput {
private:
	std::fstream f;
	std::map<std::string, double> paramsMap;
	std::string filename;
public:
	paramsInput();
	bool readFile(const std::string &filename);
	bool hasParam(const std::string &paramName);
	double getValue(const std::string &paramName, double defaultValue);
	bool printMap(const std::string &filename);
};