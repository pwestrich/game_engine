
#include <iostream>

#include "GameManager.h"
#include "NetworkManager.h"
#include "NetworkGetThread.hpp"
//#include "NetworkSendThread.hpp"

using namespace std;

//starts a connection to the specified server and port
NetworkManager::NetworkManager(GameManager *gm, const string &_serverAddress, const int _port){

	//check arguments
	assert(gm);
	assert(_port > 1024);
	assert(_port < 32000);

	//set instance variables
	gameManager = gm;
	serverAddress = _serverAddress;
	serverPort = _port;

	//set up the network connection
	address = new Poco::Net::SocketAddress(serverAddress, serverPort);

	try {

		socket = new Poco::Net::StreamSocket(*address);

	} catch (...){

		gameManager->logFatal("Could not make a connection to the server.", __LINE__, __FILE__);

	}

	//set up data sender and getter
	getter = new NetworkGetThread("Network Get Thread", this, socket);
	//sender = new NetworkSendThread("Network Send Thread", this, socket);

}

NetworkManager::~NetworkManager(){

	delete address;
	delete socket;

	//delete sender;
	delete getter;

}

bool NetworkManager::send(const char *data, const int dataSize){

	int bytesSent = socket->sendBytes(data, dataSize);
	
	if (bytesSent > 0){

		gameManager->logInfo(string("Message sent: ") + data);

	}

}

void NetworkManager::startListening(){

	getter->start();

}

void NetworkManager::messageReceived(const char *message, const int messageLength){

	gameManager->messageReceived(message, messageLength);

}
