
#ifndef LOG_MANAGER
#define LOG_MANAGER

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

enum LogLevel {LOG_ALL, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_FATAL};

class GameManager;

class LogManager {

private:

	//reference to the game manager so we can communicate to the other managers
	GameManager *gameManager;

	//ofstream object we're logging to
	ofstream outFile;

	//current logging level
	LogLevel currentLevel;

	char *getCurrentTimeString();

public:

	LogManager(GameManager *gm);
	LogManager(GameManager *gm, const string &filename, LogLevel level);
	~LogManager();

	//set the logging level
	void setLogLevel(LogLevel newLevel);

	//log something at either the default level or the given one
	void log(const string &message);
	void log(const string &message, LogLevel level);

	//or you could use these methods
	void logInfo(const string &message);
	void logWarning(const string &message);
	void logDebug(const string &message);
	void logFatal(const string &message);

};

#endif
