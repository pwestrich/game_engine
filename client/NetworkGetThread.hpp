
#ifndef NETWORK_GET_THREAD
#define NETWORK_GET_THREAD

#include <cassert>
#include <string>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class NetworkManager;

class NetworkGetThread : public Poco::Thread, public Poco::Runnable {

private:

	//keep a reference to the NetworkManager
	NetworkManager *networkManager;

	//the socket to send on
	Poco::Net::StreamSocket *socket;

	//the message to recive
	char *message;
	int messageLength;
	int messageSize;

public:

	NetworkGetThread(const string &name, NetworkManager *nm, Poco::Net::StreamSocket *_socket) : Poco::Thread(name){

		assert(nm);
		assert(_socket);

		networkManager = nm;
		socket = _socket;

		messageSize = 1025;
		messageLength = 0;

		message = new char[messageSize];
		memset(message, '\0', messageSize);

	}

	~NetworkGetThread(){}

	void start(){

		this->Thread::start(*this);

	}

	void run(){

		while (true){

			messageLength = socket->receiveBytes(message, messageSize - 1);

			if (messageLength <= 0) break; //socket closed

			networkManager->messageReceived(message, messageLength);
			memset(message, '\0', messageLength + 1);

		}

	}

};

#endif
