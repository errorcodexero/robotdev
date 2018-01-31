
#include "destination_server.h"
#include "data_logger.h"
#include <cassert>

template <typename DATATYPE>
destinationServer<DATATYPE>::destinationServer(const char *serverIP, unsigned int port) {

	//Opens and binds socket. It works...somehow
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(fd >= 0);
	

	memset((char *)&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(0);

	int res;
	res = bind(fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
	assert(res == 0);
	
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	
	res = inet_aton(serverIP, &serverAddr.sin_addr);
	assert(res != 0);


};

template <typename DATATYPE>
void destinationServer<DATATYPE>::endLoop(vector<string>& columns, vector<DATATYPE>& data) {
	for (unsigned int i = 0; i < columns.size(); i++) {
		char* buffer=new char[256];
		std::stringstream msg;
		msg << columns[i] << " " << data[i];
		strcpy(buffer, msg.str().c_str());
		ssize_t res = sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		assert(res != -1);
	}
}




#ifdef DESTINATION_SERVER_TEST

int main() {
	const char *svIP = "10.0.0.255";
	destinationServer<double> Client(svIP, 1425);
	Datalogger<double> test;
	destinationServer<double> *pClient = &Client;
	test.addDestination(pClient);
	int colindex1 = test.createColumn("col1");
	int colindex2 = test.createColumn("col2");
	test.startLoop();
	test.logData(colindex1, 42);
	test.logData(colindex2, -0.4);

	test.endLoop();
	return 0;
};
#endif
