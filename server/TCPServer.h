
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include <vector>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class TCPServer {

private:

	//keep your own address and socket
	Poco::Net::SocketAddress *serverAddress;
	Poco::Net::ServerSocket *serverSocket;

	//keep the address and socket of all the clients
	vector<Poco::Net::StreamSocket*> clientSockets;
	vector<Poco::Net::SocketAddress*> clientAddresses;

	//and the threads that they run in
	vector<Poco::Thread*> clientThreads;

public:

	//create a new server listening on the specified address with backlog pending connections
	TCPServer(const string &_address, const int _backlog);
	~TCPServer();

	//starts the server
	void start();

};


#endif

