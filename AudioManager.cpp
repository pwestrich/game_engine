
#include "GameManager.h"
#include "AudioManager.h"
#include "AudioPlayer.h"

using namespace std;

//struct to hold BASS-specific stuff hidden from other managers
struct AudioResourceInfo {

	HSAMPLE data;
	HSAMPLE channelData;

	AudioType type;

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
void AudioManager::loadAudioSample(const string &filename, AudioResourceInfo *info){

	info->data = BASS_SampleLoad(false, filename.c_str(), 0, 0, 1, 0);

	if (info->data){

		info->channelData = BASS_SampleGetChannel(info->data, false);
		info->type = A_SAMPLE;

	} else {

		gameManager->logFatal("Error loading audio resource.", __LINE__, __FILE__);

	}

}

void AudioManager::loadAudioStream(const string &filename, AudioResourceInfo *info){

	info->data = BASS_StreamCreateFile(false, filename.c_str(), 0, 0, 0);

	if (info->data){

		info->channelData = info->data;
		info->type = A_STREAM;

	} else {

		gameManager->logFatal("Error loading audio resource.", __LINE__, __FILE__);

	}

}

void AudioManager::loadAudio(const string &filename, AudioResourceInfo *info){

	assert(info != NULL);

	if (info->type == A_STREAM){

		loadAudioStream(filename, info);

	} else if (info->type == A_SAMPLE){

		loadAudioSample(filename, info);

	} else {

		gameManager->logFatal("ERROR: Invalid audio type!", __LINE__, __FILE__);

	}

}

void AudioManager::unloadAudio(AudioResourceInfo *info){

	assert(info != NULL);

	if (info->type == A_STREAM){

		unloadAudioStream(info);

	} else if (info->type == A_SAMPLE){

		unloadAudioSample(info);

	} else {

		gameManager->logFatal("Error: Cannot unload unloaded audio!", __LINE__, __FILE__);

	}

}

void AudioManager::unloadAudioSample(AudioResourceInfo *info){

	BASS_SampleFree(info->data);
	info->data = 0;
	info->channelData = 0;
	info->type = A_NONE;

}

void AudioManager::unloadAudioStream(AudioResourceInfo *info){

	BASS_StreamFree(info->data);
	info->data = 0;
	info->channelData = 0;
	info->type = A_NONE;

}

AudioResourceInfo *AudioManager::createAudioInfo(){

	AudioResourceInfo *info = new AudioResourceInfo;

	info->data = 0;
	info->channelData = 0;
	info->type = A_NONE;

	return info;

}

//methods for playing audio
void AudioManager::updateAudio(){

	//loop through every player and update
	for (size_t i = 0; i < players.size(); ++i){

		AudioPlayer *player = players[i];
		AudioResourceInfo *info = player->getResourceInfo();

		//only check stopped sounds
		if (BASS_ChannelIsActive(info->channelData) == BASS_ACTIVE_STOPPED){

			//if we've played it enough, stop playing and remove its player
			if (player->getRepeatCount() >= player->getNumRepeats()){

				player->onComplete();
				players.erase(players.begin() + i);
				--i;

			//otherwise, play it again
			} else {

				player->incrementPlayCount();
				BASS_ChannelPlay(info->channelData, false);

			}

		}

	}


}

void AudioManager::playAudio(AudioResourceInfo *info, const int numRepeats){

	assert(info != NULL);
	assert(numRepeats > 0);

	//make a new player and store it
	players.push_back(new AudioPlayer(info, numRepeats));

}
