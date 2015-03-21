
#include "GameManager.h"
#include "AudioManager.h"

using namespace std;

//struct to hold BASS-specific stuff hidden from other managers
struct AudioResourceInfo {

	HSAMPLE data;
	HSAMPLE channelData;

};

AudioManager::AudioManager(GameManager *gm){

	assert(gm != NULL);

	gameManager = gm;

	//start up the audio library
	if (BASS_Init(1, 44100, 0, 0, NULL)){

		if (BASS_GetDeviceInfo(1, &deviceInfo)){

			stringstream deviceStringStream;

			deviceStringStream << "Name: " << deviceInfo.name << "Driver: " << deviceInfo.driver << endl;
			deviceInfoString = deviceStringStream.str();
			gameManager->logInfo(deviceInfoString);

		} else {

			BASS_Free();
			gameManager->logFatal("Error while loading audio device info.", __LINE__, __FILE__);

		}

	} else {

		gameManager->logFatal("Error while loading audio.", __LINE__, __FILE__);

	}

	gameManager->logInfo("AudioManager initialized successfully!");

}

AudioManager::~AudioManager(){

	BASS_Free();

}

//methods for loading and unloading ausio resources
void AudioManager::loadAudioSample(const string &filename){



}

void AudioManager::loadSudioStream(const string &filename){



}

void AudioManager::unloadAudioSample(const AudioResourceInfo *info){



}

void AudioManager::unloadAudioStream(const AudioResourceInfo *info){



}

AudioResourceInfo *AudioManager::createAudioInfo(){

	AudioResourceInfo *info = new AudioResourceInfo;

	info->data = 0;
	info->channelData = 0;

	return info;

}

//methods for playing audio
void AudioManager::updateAudio(){



}

void AudioManager::playAudio(){



}
