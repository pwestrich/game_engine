
#include <iostream>

#include "TCPServer.h"
#include "TCPConnectionThread.h"

using namespace std;

TCPConnectionThread::TCPConnectionThread(const string &name, TCPServer *_server, Poco::Net::StreamSocket *_mySocket, int _index) :
					Poco::Thread(name){

	assert(_server);
	assert(_mySocket);

	server = _server;
	mySocket = _mySocket;
	index = _index;

	bufferSize = 1025;
	bufferLength = 0;
	buffer = new char[bufferSize];

	memset(buffer, '\0', bufferSize);

}
	
TCPConnectionThread::~TCPConnectionThread(){

	delete [] buffer;
	buffer = NULL;
	server = NULL;
	mySocket = NULL;
	bufferLength = 0;
	bufferSize = 0;
	index = 0;

}

//overloading the run method in Thread
void TCPConnectionThread::start(){

	this->Thread::start(*this);

}

//rewuired by the Runnable interface
void TCPConnectionThread::run(){

	cout << "Thread: " << getName() << " started." << endl;

	while (true){

		//wait for some data to come in
		bufferLength = mySocket->receiveBytes(buffer,  bufferSize - bufferLength);

		if (bufferLength <= 0) break; //connection closed, quit listening.

		//print message and clear buffer for now.
		cerr << getName() << ": message recived: " << buffer << endl;
		server->broadcast(buffer, bufferLength, mySocket);
		memset(buffer, '\0', bufferLength + 1);
		bufferLength = 0;

	}

	cout << getName() << " closed." << endl;
	server->removeConnection(index);

}
