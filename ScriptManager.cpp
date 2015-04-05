
#include "ScriptManager.h"
#include "GameManager.h"
#include "utilities.h"

using namespace std;

ScriptManager::ScriptManager(GameManager *gm){

	assert(gm != NULL);
	gameManager = gm;

	//set up the lua interpreter
	L = luaL_newstate();
	luaL_openlibs(L);

}

ScriptManager::~ScriptManager(){

	//close the interpreter
	lua_close(L);

}

char **ScriptManager::execute(const char *filename, const char *function, const char *opt, ...){

	//check arguments
	assert(filename != NULL);
	assert(function != NULL);
	assert(opt != NULL);

	//set up the variable args stuff
	va_list argList;
	va_start(argList, opt);
	int argc = 0;
	int retc = 0;

	//prepare the lua function
	luaL_dofile(L, filename);
	lua_getglobal(L, function);

	//get the arguments and push them to the lua stack
	while (true){

		switch (*opt++){

			case 's':{ //string

				lua_pushstring(L, va_arg(argList, char *));
				break;

			}

			case 'i':{ //int

				lua_pushnumber(L, va_arg(argList, int));
				break;

			}

			case 'd':{ //double

				lua_pushnumber(L, va_arg(argList, double));
				break;

			}

			case ':':{ //enf of arguments

				goto end;
				break;

			}

			default:{
				
				gameManager->logWarn("Invalid argument type in ScriptManager.");
				return NULL;
			}

		}

		//increment the argument count, and make sure that there are no overflows
		argc++;
		//lua_checkstack(L, 1, "too many arguments");

	}

end:
	
	retc = strlen(opt); //count return values

	//call the function
	if (lua_pcall(L, argc, retc, 0) != 0){

		gameManager->logWarn("Error calling Lua function.");
		return NULL;

	}

	if (retc <= 1){

		return NULL; //no return values

	}

	//now get return values
	char **retv = new char*[retc - 1];
	retc = -retc;

	while (true){

		switch (*opt++){

			case 's':{ //string

				if (!lua_isstring(L, retc)){

					gameManager->logFatal("Invalid script return type: s", __LINE__, __FILE__);

				}

				const char *value = lua_tostring(L, retc);
				retv[-retc - 2] = new char[strlen(value)];
				strcpy(retv[-retc - 2], value);
				break;

			}

			case 'i':{ //int

				if (!lua_isnumber(L, retc)){

					gameManager->logFatal("Invalid script return type: i", __LINE__, __FILE__);

				}

				char *value = itoa(lua_tonumber(L, retc));
				retv[-retc - 2] = new char[strlen(value)];
				strcpy(retv[-retc - 2], value);
				break;

			}

			case 'd':{ //double

				if (!lua_isnumber(L, retc)){

					gameManager->logFatal("Invalid script return type: d", __LINE__, __FILE__);

				}

				char *value = dtoa(lua_tonumber(L, retc));
				retv[-retc - 2] = new char[strlen(value)];
				strcpy(retv[-retc - 2], value);
				break;


			}

			case '.':{

				goto done;

			}

			default: { //oh no

				gameManager->logFatal("Invalid return type", __LINE__, __FILE__);

			}

		}

		retc++;

	}

done:
	
	va_end(argList);
	return retv;

}
