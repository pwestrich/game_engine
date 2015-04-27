
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "TCPServer.h"

using namespace std;

//create a new server listening on the specified address with backlog pending connections
TCPServer::TCPServer(const string &_address, const int _backlog){

	assert(_backlog > 0);

	serverAddress = new Poco::Net::SocketAddress(_address, 12345);
	serverSocket = new Poco::Net::ServerSocket();

	serverSocket->bind(*serverAddress);
	serverSocket->listen(_backlog);

}

TCPServer::~TCPServer(){

	delete serverSocket;
	delete serverAddress;

}

//starts the server
void TCPServer::start(){

	Poco::Net::SocketAddress *newClientAddress = NULL;
	Poco::Net::StreamSocket *newClientSocket = NULL;
	Poco::Thread *newClientThread = NULL;

	while (true) {

		cout << "Awaiting connection..." << endl;

		newClientAddress = new Poco::Net::SocketAddress();
		newClientSocket = new Poco::Net::StreamSocket(serverSocket->acceptConnection(*newClientAddress));

		cout << "Connection established: " << newClientAddress->host().toString() << endl;


	}

}
