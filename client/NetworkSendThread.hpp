
#ifndef NETWORK_SEND_H
#define NETWORK_SEND_H

#include <cassert>
#include <string>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class NetworkManager;

class NetworkSendThread : public Poco::Thread, public Poco::Runnable {

private:

	//keep a reference to the NetworkManager
	NetworkManager *networkManager;

	//the socket to send on
	Poco::Net::StreamSocket *socket;

	//the message to send
	char *message;
	int messageLength;

public:

	NetworkSendThread(const string &name, NetworkManager *nm, Poco::Net::StreamSocket *_socket) : Poco::Thread(name) {

		assert(nm);
		assert(socket);

		networkManager = nm;
		socket = _socket;

	}

	~NetworkSendThread(){}

	//overloading the start method
	void start(const char *_message, const int _messageLength){

		assert(_message);

		message = const_cast<char*>(_message);
		messageLength = _messageLength;

		this->Thread::start(*this);

	}

	//required from the Runnable interface
	void run(){

		int bytesSent = 0;

		while (bytesSent < messageLength){

			bytesSent += socket->sendBytes(message + bytesSent, messageLength - bytesSent);

		}

	}

};

#endif
