
#include "GameManager.h"
#include "LogManager.h"

using namespace std;

LogManager::LogManager(GameManager *gm) : LogManager(gm, "game.log", LOG_WARN){}

LogManager::LogManager(GameManager *gm, const string &filename, LogLevel level){

	gameManager = gm;
	currentLevel = level;
	outFile.open(filename);

	if (!outFile){

		cerr << "FATAL: Log file " << filename << "could not be opened." << endl;
		exit(EXIT_FAILURE);

	}

}

LogManager::~LogManager(){

	outFile.close();

}

void LogManager::setLogLevel(LogLevel newLevel){

	currentLevel = newLevel;

}

void LogManager::log(const string &message){



}

void LogManager::log(const string &message, LogLevel level){


}

void LogManager::logInfo(const string &message){


}

void LogManager::logWarning(const string &message){


}

void LogManager::logDebug(const string &message){


}

void LogManager::logFatal(const string &message){


}
