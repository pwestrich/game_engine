
#ifndef NETWORK_MANAGER
#define NETWORK_MANAGER

#include <string>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class GameManager;

class NetworkManager : public Poco::Runnable {

private:

	//reference to GameManager
	GameManager *gameManager;
	
	//buffers for sending and reciving
	char *sendBuffer;
	char* receiveBuffer;

	//the current amount of meaningful data in the buffer
	int sendBufferLength;
	int receiveBufferLength;

	//the maximum size of the buffers
	int sendBufferSize;
	int receiveBufferSize;

	//server information
	string serverAddress;
	int serverPort;

	Poco::Net::StreamSocket *socket;
	Poco::Net::SocketAddress *address;

	Poco::Thread *thread;

public:

	//starts a connection to the specified server and port
	NetworkManager(GameManager *gm, const string &_serverAddress, const int _port);
	~NetworkManager();

	//adds a piece of data to the send buffer
	void send(const void *data, const int dataSize);

	//gets the contents of the recieve buffer and clears it
	//returns NULL if no data in buffer, and dataSize will be set to zero
	void *receive(int &dataSize);

	//required by Poco::Runnable
	void run();

};

#endif
