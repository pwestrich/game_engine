
#ifndef AUDIO_PLAYER
#define AUDIO_PLAYER

#include <cstdlib>
#include <cassert>

using namespace std;

struct AudioResourceInfo;

class AudioPlayer {

private:

	AudioResourceInfo *info;

	int repeatCount;
	int numRepeats;

public:

	AudioPlayer(AudioResourceInfo *newInfo, const int playCount);
	~AudioPlayer();

	int getNumRepeats();
	int getRepeatCount();
	AudioResourceInfo *getResourceInfo();

	void incrementPlayCount();

	void onComplete();

};

#endif
