
#include "AudioPlayer.h"

using namespace std;

AudioPlayer::AudioPlayer(AudioResourceInfo *newInfo, const int playCount){

	assert(newInfo != NULL);
	assert(playCount > 0);

	info = newInfo;
	repeatCount = 0;
	numRepeats = playCount;

}

AudioPlayer::~AudioPlayer(){}

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
	delete this;

}

