#include "AudioManager.h"

std::vector<std::pair<char*, uint32_t>> AudioManager::soundLists_;

AudioManager::AudioManager()
{
	audio_ = Audio::GetInstance();
}

void AudioManager::PlayAudio(uint32_t soundHandle, bool isLoop, float volume)
{
	if (!Audio::GetInstance()->IsPlaying(soundHandle)) {
		Audio::GetInstance()->PlayWave(soundHandle, isLoop, volume);
	}
	else {
		Audio::GetInstance()->StopWave(soundHandle);
		Audio::GetInstance()->PlayWave(soundHandle, isLoop, volume);
	}
}

void AudioManager::PlayAudio(const char* soundID, bool isLoop, float volume)
{
	Audio::GetInstance()->PlayWave(GetSound(soundID), isLoop, volume);
}

uint32_t AudioManager::GetSound(const char* soundID)
{
	for (const auto& sound : soundLists_) {
		if (sound.first == soundID) {
			return sound.second;
		}
	}
	return 0;
}

void AudioManager::AddSound(uint32_t soundHandle, const char* soundID)
{
	soundLists_.push_back(std::make_pair(const_cast<char*>(soundID), soundHandle));
}
