
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include "ScriptManager.h"
#include "GameManager.h"

#include "luainc.h"
#include "LuaContext.hpp"

using namespace std;

ScriptManager::ScriptManager(GameManager *gm){

	assert(gm != NULL);
	gameManager = gm;

	//set up the lua interpreter
	lua = new LuaContext();

}

ScriptManager::~ScriptManager(){

	delete lua;

}

LuaContext *ScriptManager::getLuaContext(){

	return lua;

}

void ScriptManager::execute(const string &filename){

	ifstream inFile(filename);

	if (!inFile){

		gameManager->logWarn("Script could not be run.");
		gameManager->logWarn(filename);
		return;

	}

	try {
	
		lua->executeCode(inFile);
		inFile.close();

	} catch (runtime_error &it){

		gameManager->logWarn(it.what());

	} 

}

//templates wouldn't work, so there's several of this method...
void ScriptManager::writeInt(const string &name, const int value){

	lua->writeVariable(name, value);

}

void ScriptManager::writeFloat(const string &name, const float value){

	lua->writeVariable(name, value);

}

void ScriptManager::writeString(const string &name, const string &value){

	lua->writeVariable(name, value);

}
