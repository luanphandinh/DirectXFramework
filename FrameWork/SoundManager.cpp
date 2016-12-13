

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

	sound = LoadSound("Resources//Sound//Game_Start_Prologue.wav");
	_listSound[eSoundId::BACKGROUND_MENU] = sound;

	sound = LoadSound("Resources//Sound//usingwhip.wav");
	_listSound[eSoundId::USINGWHIP] = sound;

	sound = LoadSound("Resources//Sound//hit.wav");
	_listSound[eSoundId::HIT] = sound;

	sound = LoadSound("Resources//Sound//opendoor.wav");
	_listSound[eSoundId::OPENDOOR] = sound;

	sound = LoadSound("Resources//Sound//collectweapon.wav");
	_listSound[eSoundId::COLLECTITEM] = sound;

	sound = LoadSound("Resources//Sound//collectweapon.wav");
	_listSound[eSoundId::COLLECTWEAPON] = sound;
	

	sound = LoadSound("Resources//Sound//Boss_Battle_Poison_Mind.wav");
	_listSound[eSoundId::BOSSMEDUSA] = sound;
	
	sound = LoadSound("Resources//Sound//hitcannon.wav");
	_listSound[eSoundId::HITCANNON] = sound;
	//sound = LoadSound("Resources//Sound//");
	//_listSound[eSoundId::USINGWHIP] = sound;
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