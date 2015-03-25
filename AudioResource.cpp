
#include "AudioResource.h"
#include "GameManager.h"

using namespace std;

AudioResource::AudioResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm, AudioResourceInfo *newInfo) 
								: GameResource(newID, newGroup, newName, gm, RT_AUDIO) {

	assert(newInfo != NULL);

	info = newInfo;

}

AudioResource::~AudioResource(){};

AudioResourceInfo *AudioResource::getInfo(){

	return info;

}

//methods from GameResource
void AudioResource::load(){

	gameManager->logInfo("Loading AudioResource");
	gameManager->loadAudio(getFilename(), info);
	loaded = true;

}

void AudioResource::unload(){

	gameManager->unloadAudio(info);
	loaded = false;

}
