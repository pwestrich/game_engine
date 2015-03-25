
#include <iostream>

#include "GameManager.h"

using namespace std;

int main(int argc, char **argv){

	GameManager *manager = GameManager::getGameManager();

	delete manager;
	
	return 0;

}
