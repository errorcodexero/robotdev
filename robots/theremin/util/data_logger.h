using namespace std;
#include <vector>
#include <iostream>
#include <string>

	class Datalogger {
	private:
		vector<string> columns;
		vector<double> data;
	public:
		Datalogger()
		{

		}
		int createColumn(string name);
		void startLoop();
		void logData(int colnumber, double value);
		void endLoop();
	};