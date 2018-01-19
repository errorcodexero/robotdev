#include "paramsParser.h"
#include <ostream>

paramsInput::paramsInput()
{
}

bool paramsInput::readFile(const std::string &filename)
{
    bool ret = false ;
    this->mFilename = filename;

    std::fstream f(filename) ;
    if (f.is_open())
    {
	std::string buffer;
	
	while (!f.eof())
	{
	    std::getline(f, buffer);
	    unsigned int commentStart = buffer.find('#');
      
	    if (commentStart != buffer.npos)
	    {
		buffer = buffer.substr(0, commentStart);
	    }
      
	    if (buffer.length() > 0)
	    {
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
		mParamsMap.insert(std::pair<std::string, double>(Key, value));
	    }
	}
	ret = true ;
    }

    return ret ;
}

bool paramsInput::printMap(const std::ostream &fo)
{
    for (auto it = mParamsMap.begin() ; it != mParamsMap.end(); it++)
    {
	fo << it->first << ' ' << it.second << std::endl;
    }
    
    return true ;
}

bool paramsInput::hasParam(const std::string &paramName)
{
    bool found = false;
    if (mParamsMap.find(paramName) != mParamsMap.end())
	found = true;
    
    return found;
}

double paramsInput::getValue(const std::string &paramName, double defaultValue)
{
    auto it = mParamsMap.find(paramName) ;
    if (it == mParamsMap.end())
	return defaultValue ;
    
    return it->second ;
}
