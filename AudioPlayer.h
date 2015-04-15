
#ifndef AUDIO_PLAYER
#define AUDIO_PLAYER

#include <cstdlib>
#include <cstdint>
#include <cassert>

using namespace std;

struct AudioResourceInfo;

class AudioPlayer {

private:

	AudioResourceInfo *info;

	uint32_t id;

	int repeatCount;
	int numRepeats;

public:

	AudioPlayer(AudioResourceInfo *newInfo, const int playCount, uint32_t _id);
	~AudioPlayer();

	uint32_t getID();
	int getNumRepeats();
	int getRepeatCount();
	AudioResourceInfo *getResourceInfo();

	void incrementPlayCount();

	void onComplete();

};

#endif
