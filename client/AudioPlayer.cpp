
#include "AudioPlayer.h"

using namespace std;

AudioPlayer::AudioPlayer(AudioResourceInfo *newInfo, const int playCount, uint32_t _id){

	assert(newInfo != NULL);
	assert(playCount > 0);

	id = _id;
	info = newInfo;
	repeatCount = 0;
	numRepeats = playCount;

}

AudioPlayer::~AudioPlayer(){}

uint32_t AudioPlayer::getID(){

	return id;

}

int AudioPlayer::getNumRepeats(){

	return numRepeats;

}

int AudioPlayer::getRepeatCount(){

	return repeatCount;

}

AudioResourceInfo *AudioPlayer::getResourceInfo(){

	return info;

}

void AudioPlayer::incrementPlayCount(){

	repeatCount++;

}

void AudioPlayer::onComplete(){

	//do nothing for now

}

