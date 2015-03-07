
#include "GameManager.h"
#include "LogManager.h"

using namespace std;

LogManager::LogManager(GameManager *gm) : LogManager(gm, "game.log", LOG_ALL){}

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

	log(message, currentLevel);

}

void LogManager::log(const string &message, LogLevel level){

	//just log everything for noe without any logic
	if (level >= currentLevel){

		clog << getCurrentTimeString();
		outFile << getCurrentTimeString();

		switch (level){

			case LOG_INFO:

				clog << ": INFO: ";
				outFile << ": INFO: ";
				break;

			case LOG_WARN:

				clog << ": WARNING: ";
				outFile << ": WARNING: ";
				break;

			case LOG_DEBUG:

				clog << ": DEBUG INFO: ";
				outFile << ": DEBUG INFO: ";
				break;
		}

		clog << message << endl;
		outFile << message << endl;

	}

}

void LogManager::logInfo(const string &message){

	log(message, LOG_INFO);

}

void LogManager::logWarning(const string &message){

	log(message, LOG_WARN);

}

void LogManager::logDebug(const string &message){

	log(message, LOG_DEBUG);

}

void LogManager::logFatal(const string &message, const int line, const char *file){

	clog << "FATAL ERROR at " << file << ":" << line << ": " << message << endl;
	outFile << "FATAL ERROR at " << file << ":" << line << ": " << message << endl;
	outFile.close();
	exit(EXIT_FAILURE);

}

char *LogManager::getCurrentTimeString(){

	char *buffer = new char[128];
	time_t rawTime;
	struct tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(buffer, 128, "%Y/%m/%d %H:%M:%S", timeInfo);

	return buffer;

}
