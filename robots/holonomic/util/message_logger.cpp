#include "message_logger.h"

MessageLogger::MessageLogger()
{
	col_ix = data_logger.createColumn("message");
}


void MessageLogger::addDestination(DataloggerDestination<string>* new_destination)
{
	data_logger.addDestination(new_destination);
}

void MessageLogger::logMessage(string msg)
{
	data_logger.startLoop();
	data_logger.logData(col_ix, msg);
	data_logger.endLoop();
}
