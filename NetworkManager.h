
#ifndef NETWORK_MANAGER
#define NETWORK_MANAGER

#include <string>

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

using namespace std;

class GameManager;

class NetworkManager {

private:

	class DataSendThread : public Poco::Runnable {

	private:

		NetworkManager *networkManager;

	public:

		DataSendThread(NetworkManager *nm) { networkManager = nm; };
		void run();

	};

	class DataGetThread : public Poco::Runnable {
	
	private:

		NetworkManager *networkManager;

	public:

		DataGetThread(NetworkManager *nm) { networkManager = nm; };
		void run();

	};

	friend class DataGetThread;
	friend class DataSendThread;

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

	Poco::Thread *sendThread;
	Poco::Thread *getThread;

	DataGetThread *getter;
	DataSendThread *sender;

	bool sendLocked;
	bool receiveLocked;

public:

	//starts a connection to the specified server and port
	NetworkManager(GameManager *gm, const string &_serverAddress, const int _port);
	~NetworkManager();

	//adds a piece of data to the send buffer
	bool send(const void *data, const int dataSize);

	//gets the contents of the recieve buffer and clears it
	//returns NULL if no data in buffer, and dataSize will be set to zero
	void *receive(int &dataSize);

};

#endif
