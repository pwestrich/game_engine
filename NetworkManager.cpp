
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


}

NetworkManager::~NetworkManager(){

	delete [] sendBuffer;
	delete [] receiveBuffer;

}

//adds a piece of data to the send buffer
void NetworkManager::send(const void *data, const int dataSize){



}

//gets the contents of the recieve buffer and clears it
//returns NULL if no data in buffer, and dataSize will be set to zero
void *NetworkManager::receive(int &dataSize){



}

//required by Poco::Runnable
void NetworkManager::run(){



}
