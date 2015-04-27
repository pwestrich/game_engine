
#include <cstdlib>
#include <iostream>

#include "TCPServer.h"

using namespace std;

int main(const int argc, const char **argv){

	TCPServer *server = new TCPServer("localhost", 8);
	server->start();
	delete server;

	return 0;

}
