
#ifndef TCP_CONNECTION_THREAD_H
#define TCP_CONNECTION_THREAD_H

#include <cassert>
#include <string>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class TCPServer;

class TCPConnectionThread : public Poco::Thread, public Poco::Runnable {

private:

	//keep a pointer to the server
	TCPServer *server;

	//keep your own socket
	Poco::Net::StreamSocket *mySocket;
	
	//buffer for reciving messages
	char *buffer;
	int bufferSize;
	int bufferLength;

public:

	TCPConnectionThread(const string &name, TCPServer *_server, Poco::Net::StreamSocket *_mySocket);
	~TCPConnectionThread();

	//overloading the run method in Thread
	void start();

	//rewuired by the Runnable interface
	void run();

};

#endif
