
#include "AudioResource.h"
#include "AudioManager.h"
#include "GameManager.h"

using namespace std;

AudioResource::AudioResource(uint32_t newID, const string &newGroup, const string &newName, GameManager *gm, AudioResourceInfo *newInfo, const string &newType) 
								: GameResource(newID, newGroup, newName, gm, RT_AUDIO) {

	assert(newInfo != NULL);

	info = newInfo;
	type = newType;

}

AudioResource::~AudioResource(){};

AudioResourceInfo *AudioResource::getInfo(){

	return info;

}

//methods from GameResource
void AudioResource::load(){

	gameManager->logInfo("Loading AudioResource");

	if (type == "stream"){

		gameManager->loadAudioStream(getFilename(), info);

	} else if (type == "sample"){

		gameManager->loadAudioSample(getFilename(), info);

	} else {

		gameManager->logFatal("Invalid audio type!", __LINE__, __FILE__);

	}

	loaded = true;

}

void AudioResource::unload(){

	gameManager->unloadAudio(info);
	loaded = false;

}
