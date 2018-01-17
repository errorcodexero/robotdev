#include "Datalogger.h"

int Datalogger::createColumn(string name)
{
	columns.push_back(name);
	return columns.size()-1;
}

void Datalogger::startLoop()
{
	data.resize(columns.size());
	data_entered.resize(columns.size());

	data_entered.assign(data_entered.size(), false);

}

void Datalogger::logData(int colnumber, double value)
{
	data[colnumber] = value;
	data_entered[colnumber] = true;
}

void Datalogger::endLoop()
{
	for (size_t i = 0; i < columns.size(); i++)
	{
		cout << columns[i] << "=";
		if (data_entered[i]) {
			cout << data[i];
		}
		
		if (i < columns.size() - 1) {
			cout << "  ";
		}
	}
	cout << endl;
}
