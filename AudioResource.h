
#ifndef AUDIO_RESOURCE
#define AUDIO_RESOURCE

#include "GameResource.h"

using namespace std;

struct AudioResourceInfo;

class AudioResource : public GameResource {

private:

	AudioResourceInfo *info;

public:

	AudioResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm, AudioResourceInfo *newInfo);
	~AudioResource();

	AudioResourceInfo *getInfo();

	//methods from GameResource
	void load();
	void unload();

};

#endif
