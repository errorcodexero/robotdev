#pragma once

#include <fstream>
#include <vector>

using namespace std;

template <typename DATATYPE>
class DataloggerDestination {
private:

public:
	virtual ~DataloggerDestination() =0;
	virtual void endLoop(vector<string>& columns, vector<DATATYPE>& data) =0;
};


template <typename DATATYPE>
DataloggerDestination<DATATYPE>::~DataloggerDestination()
{
	// Empty implementation required for destructor even though it's pure virtual
}

