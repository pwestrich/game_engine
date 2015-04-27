
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "TCPServer.h"
#include "TCPConnectionThread.h"

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
	int numThreads = 0;

	while (true) {

		//wait for a connection
		cout << "Awaiting connection..." << endl;
		newClientAddress = new Poco::Net::SocketAddress();
		newClientSocket = new Poco::Net::StreamSocket(serverSocket->acceptConnection(*newClientAddress));

		//now set up a new thread for that connection 
		cout << "Connection established from: " << newClientAddress->host().toString() << ":" << newClientAddress->port() << endl;
		clientSockets.push_back(newClientSocket);
		clientAddresses.push_back(newClientAddress);
		clientThreads.push_back(new TCPConnectionThread(nextThreadName(), this, newClientSocket, clientThreads.size()));
		clientThreads.back()->start();

	}

}

void TCPServer::broadcast(char *data, int dataSize, Poco::Net::StreamSocket *exclude){

	for (size_t i = 0; i < clientSockets.size(); ++i){

		if (clientSockets[i] == exclude) continue;

		clientSockets[i]->sendBytes(data, dataSize);

	}

}

void TCPServer::removeConnection(const int index){

	clientAddresses.erase(clientAddresses.begin() + index);
	clientSockets.erase(clientSockets.begin() + index);
	clientThreads.erase(clientThreads.begin() + index);

}

//private methods below here ----------------------------------------------------------------------
string TCPServer::nextThreadName(){

	static int threadNum = 0;
	stringstream ss;

	ss << "Client thread #" << threadNum++;
	return ss.str();

}
