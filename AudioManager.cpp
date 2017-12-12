#include "AudioManager.h"

AudioManager* AudioManager::sInstance = NULL;

//create instance
AudioManager* AudioManager::Instance() {

	if (sInstance == NULL) {
		sInstance = new AudioManager();
	}

	return sInstance;
}

//release
void AudioManager::Release() {

	delete sInstance;
	sInstance = NULL;
}

//constructor
AudioManager::AudioManager() {

	mAssetMgr = AssetManager::Instance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {

		printf("Mixer Initialization Error: %s\n", Mix_GetError());
	}
}

//destructor
AudioManager::~AudioManager() {

	mAssetMgr = NULL;
	Mix_Quit();
}

//play music
void AudioManager::PlayMusic(std::string fileName, int loops) {

	Mix_PlayMusic(mAssetMgr->GetMusic(fileName), loops);
}

//pause music
void AudioManager::PauseMusic() {

	if (Mix_PlayingMusic() != 0) {
		Mix_PauseMusic();
	}
}

//resume music
void AudioManager::ResumeMusic() {

	if (Mix_PausedMusic() != 0) {
		Mix_ResumeMusic();
	}
}

//play sfx
void AudioManager::PlaySFX(std::string fileName, int loops, int channel) {

	Mix_PlayChannel(channel, mAssetMgr->GetSFX(fileName), loops);
}