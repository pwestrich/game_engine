
#include <iostream>

#include "GameManager.h"
#include "NetworkManager.h"

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

	sendBufferSize = 1025;
	receiveBufferSize = 1025;

	sendBuffer = new char[sendBufferSize];
	receiveBuffer = new char[receiveBufferSize];

	memset(sendBuffer, '\0', sendBufferSize);
	memset(receiveBuffer, '\0', receiveBufferSize);

	sendBufferLength = 0;
	receiveBufferLength = 0;

	sendLocked = false;
	receiveLocked = false;

	//set up the network connection
	address = new Poco::Net::SocketAddress(serverAddress, serverPort);
	socket = new Poco::Net::StreamSocket(*address);

	//set up data sender and getter
	getter = new DataGetThread(this);
	sender = new DataSendThread(this);

	sendThread = new Poco::Thread("Network Send Thread");
	getThread  = new Poco::Thread("Network Get Thread");

	sendThread->start(*sender);
	getThread->start(*getter);

}

NetworkManager::~NetworkManager(){

	delete address;
	delete socket;

	delete sender;
	delete getter;

	delete sendThread;
	delete getThread;

	delete [] sendBuffer;
	delete [] receiveBuffer;

}

//adds a piece of data to the send buffer
bool NetworkManager::send(const void *data, const int dataSize){

	//make sure there is enough room in the buffer for data
	if (dataSize > sendBufferSize - sendBufferLength){

		return false;

	} else if (sendLocked) {

		return false;

	} else {

		sendLocked = true;

		memcpy(sendBuffer + sendBufferLength, data, dataSize);
		sendBufferLength += dataSize;

		sendLocked = false;
		return true;

	}

}

//gets the contents of the recieve buffer and clears it
//returns NULL if no data in buffer, and dataSize will be set to zero
void *NetworkManager::receive(int &dataSize){

	if (receiveLocked){

		//do nothing if the buffer is locked
		dataSize = 0;
		return NULL;

	} else if (receiveBufferLength == 0){

		//do nothing if there is nothing to get
		dataSize = 0;
		return NULL;

	} else {

		receiveLocked = true;

		//copy data from the buffer into a new array
		dataSize = receiveBufferLength + 1;
		char *data = new char[dataSize];
		memcpy(data, receiveBuffer, receiveBufferLength);
		data[dataSize] = '\0';

		//clear the buffer
		memset(receiveBuffer, '\0', receiveBufferLength);
		receiveBufferLength = 0;

		receiveLocked = false;

		return data;

	}

}

//implementation of the send and get threads
void NetworkManager::DataSendThread::run(){

	while (true){

		networkManager->sendThread->sleep(1000);

		if (networkManager->sendLocked){

			continue; //do not send data while someone else is using the buffer

		} else if (networkManager->sendBufferLength == 0){

			continue; //do not send if there is nothing to send either

		} else {

			networkManager->sendLocked = true;

			//send what has accumulated in the buffer
			networkManager->socket->sendBytes(networkManager->sendBuffer, networkManager->sendBufferLength);

			//clear the buffer
			memset(networkManager->sendBuffer, '\0', networkManager->sendBufferLength);
			networkManager->sendBufferLength = 0;

			networkManager->sendLocked = false;

		}

	}

}

void NetworkManager::DataGetThread::run(){

	while (true){

		networkManager->getThread->sleep(1000);

		if (networkManager->receiveLocked){

			continue; //do not fill the buffer if it is being cleared

		} else if (networkManager->receiveBufferLength >= networkManager->receiveBufferSize){

			continue; //do not recive data if the buffer is full

		} else {

			networkManager->receiveLocked = true;

			//get some data
			int bytesRecived = networkManager->socket->receiveBytes(&networkManager->receiveBuffer[networkManager->receiveBufferLength],
															(networkManager->receiveBufferSize - networkManager->receiveBufferLength));
			
			networkManager->receiveBufferLength +=bytesRecived;

			networkManager->receiveLocked = false;

		}

	}

}
