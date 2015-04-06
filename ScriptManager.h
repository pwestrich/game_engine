
#ifndef SCRIPT_MAMAGER
#define SCRIPT_MAMAGER

#include <string>

using namespace std;

class GameManager;
class LuaContext;

class ScriptManager {

private:

	GameManager *gameManager;
	LuaContext *lua;

public:

	ScriptManager(GameManager *gm);
	~ScriptManager();

	LuaContext *getLuaContext();

	void execute(const string &filename);

	/*
	This one will require some explaning...
	This function executes a function in a file, with arguments specified by opt.
	Opt can be formatted as follows: '<types_in>:<types_out>.'
	where either <types_in> or <types_out> can be s, i, d for string, int, or double.
	If there are no return types, the colon and afterwards are not needed.

	This function returns a pointer to the block in memory where the results are stored, 
	or NULL if there are none. It is up to the calling function to get the values out of it, 
	and to free the memory it uses.
	*/
	//char **execute(const char *filename, const char *function, const char *opt, ...);

};

#endif

