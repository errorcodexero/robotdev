#pragma once

using namespace std;
#include <vector>
#include <iostream>
#include <string>
#include <assert.h>

#include "data_logger.h"
#include "data_logger_destination.h"

class MessageLogger {
private:
	Datalogger<string> data_logger;
	int col_ix;
	vector< DataloggerDestination<string>* > destinations;

public:

	MessageLogger();
	void addDestination(DataloggerDestination<string>* new_destination);
	void logMessage(string msg);
};



