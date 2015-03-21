
#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER

#include <windows.h> //there goes any chance of cross-compatibillity
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>

#include "bass.h"

using namespace std;

class GameManager;
class AudioPlayer;
struct AudioResourceInfo;

enum AudioType : uint8_t { A_NONE, A_STREAM, A_SAMPLE };

class AudioManager {

private:

	//game manager for refering to the rest fo everything
	GameManager *gameManager;

	//bass stuff
	BASS_DEVICEINFO deviceInfo;
	string deviceInfoString;

	//list of players
	vector<AudioPlayer*> players;

	void unloadAudioSample(AudioResourceInfo *info);
	void unloadAudioStream(AudioResourceInfo *info);

public:

	AudioManager(GameManager *gm);
	~AudioManager();

	//mathods for playing/pausing
	void pause() { BASS_Pause(); }
	void start() { BASS_Start(); }
	void setVolume(const float volume) { BASS_SetVolume(volume); }

	//methods for loading and unloading ausio resources
	void loadAudioSample(const string &filename, AudioResourceInfo *info);
	void loadSudioStream(const string &filename, AudioResourceInfo *info);

	void unloadAudio(AudioResourceInfo *info);

	AudioResourceInfo *createAudioInfo();

	//methods for playing audio
	void updateAudio();
	void playAudio(AudioResourceInfo *info, const int numRepeats);

};

#endif
