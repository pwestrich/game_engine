
#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include <windows.h> //there goes any chance of cross-compatibillity
#include <string>
#include <sstream>

#include "bass.h"

using namespace std;

class GameManager;
struct AudioResourceInfo;

class AudioManager {

private:

	//game manager for refering to the rest fo everything
	GameManager *gameManager;

	//bass stuff
	BASS_DEVICEINFO deviceInfo;
	string deviceInfoString;

public:

	AudioManager(GameManager *gm);
	~AudioManager();

	//mathods for playing/pausing
	void pause() { BASS_Pause(); }
	void start() { BASS_Start(); }
	void setVolume(const float volume) { BASS_SetVolume(volume); }

	//methods for loading and unloading ausio resources
	void loadAudioSample(const string &filename);
	void loadSudioStream(const string &filename);

	void unloadAudioSample(const AudioResourceInfo *info);
	void unloadAudioStream(const AudioResourceInfo *info);

	AudioResourceInfo *createAudioInfo();

	//methods for playing audio
	void updateAudio();
	void playAudio();

};

#endif
