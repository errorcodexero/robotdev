#include "paramsParser.h"

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

bool paramsInput::printMap(const std::string &filename)
{
    bool ret = false ;
    std::ofstream fo;
    fo.open(filename);
    if (fo.is_open())
    {
	ret = true ;
	std::map<std::string, double>::iterator it = mParamsMap.begin();
	for (; it != mParamsMap.end(); it++)
	{
	    fo << it->first << ' ' << it->second << std::endl;
	}
	fo.close();
	ret = true ;
    }

    return ret ;
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
