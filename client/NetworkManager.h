
#ifndef NETWORK_MANAGER
#define NETWORK_MANAGER

#include <string>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"

using namespace std;

class GameManager;
class NetworkGetThread;
//class NetworkSendThread;

class NetworkManager {

private:

	//reference to GameManager
	GameManager *gameManager;
	
	//server information
	string serverAddress;
	int serverPort;

	Poco::Net::StreamSocket *socket;
	Poco::Net::SocketAddress *address;

	NetworkGetThread *getter;
	//NetworkSendThread *sender;

public:

	//starts a connection to the specified server and port
	NetworkManager(GameManager *gm, const string &_serverAddress, const int _port);
	~NetworkManager();

	//adds a piece of data to the send buffer
	bool send(const char *data, const int dataSize);

	void startListening();
	void messageReceived(const char *message, const int messageLength);

};

#endif
