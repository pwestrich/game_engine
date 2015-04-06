
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

	void writeInt(const string &name, const int value);
	void writeFloat(const string &name, const float value);
	void writeString(const string &name, const string &value);

	//runs a lua script in filename
	void execute(const string &filename);

};

#endif

