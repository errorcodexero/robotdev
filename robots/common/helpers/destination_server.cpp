
#include "destination_server.h"
#include "data_logger.h"
#include <cassert>

template <typename DATATYPE>
destinationServer<DATATYPE>::destinationServer(char *serverIP, unsigned int port) {

	//Opens and binds socket. It works...somehow
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(fd >= 0);
	

	memset((char *)&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(0);

	assert(bind(fd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) >= 0);
	
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	
	assert(inet_aton(serverIP, &serverAddr.sin_addr) != 0);


};

template <typename DATATYPE>
void destinationServer<DATATYPE>::endLoop(vector<string>& columns, vector<DATATYPE>& data) {
	for (unsigned int i = 0; i < columns.size(); i++) {
		char* buffer=new char[256];
		std::stringstream msg;
		msg << columns[i] << " " << data[i];
		strcpy(buffer, msg.str().c_str());
		assert(sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)));
	}
}




#ifdef DESTINATION_SERVER_TEST

int main() {
	char *svIP = "10.0.0.255";
	destinationServer<double> Client(svIP, 1425);
	Datalogger<double> test;
	destinationServer<double> *pClient = &Client;
	test.addDestination(pClient);
	int columnindex = test.createColumn("name");
	int columnindex1 = test.createColumn("name");
	test.startLoop();
	test.logData(0, 42);
	test.logData(1, -0.4);

	test.endLoop();
	return 0;
};
#endif