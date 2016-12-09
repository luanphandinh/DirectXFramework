

#include "SoundManager.h"

SoundManager* SoundManager::_instance;

SoundManager* SoundManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new SoundManager();
	}
	return _instance;
}

void SoundManager::loadSound(HWND hWnd) {
	// Khởi tạo CSoundManager.
	DirectSound_Init(hWnd);

	CSound* sound = nullptr;

	sound = LoadSound("Resources//Sound//05-stalker.wav");
	_listSound[eSoundId::BACKGROUND_LEVEL2] = sound;

	sound = LoadSound("Resources//Sound//06-wicked-child.wav");
	_listSound[eSoundId::BACKGROUND_LEVEL3] = sound;
}

bool SoundManager::IsPlaying(eSoundId soundid) {
	auto sound = _listSound[soundid];
	if (sound != nullptr) {
		return sound->IsSoundPlaying();
	}
	return false;
}

void SoundManager::Play(eSoundId soundid) {
	auto sound = _listSound[soundid];
	if (sound != nullptr) {
		PlaySound(sound);
		this->currentSound = sound;
	}
}
void SoundManager::PlayLoop(eSoundId soundid) {
	auto sound = _listSound[soundid];
	if (sound != nullptr) {
		sound->Play(0, DSBPLAY_LOOPING);
	}
}
void SoundManager::Stop(eSoundId soundid) {
	auto sound = _listSound[soundid];
	if (sound != nullptr) {
		StopSound(sound);
	}
}

SoundManager::SoundManager() {
	currentSound = nullptr;
}

SoundManager::~SoundManager() {}