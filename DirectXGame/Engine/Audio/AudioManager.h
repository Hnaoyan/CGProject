#pragma once
#include "Audio.h"
#include <vector>

/// <summary>
/// 音源を管理するクラス
/// </summary>
class AudioManager
{
public:
	AudioManager();
	~AudioManager() = default;

	static AudioManager* GetInstance() {
		static AudioManager instance;
		return &instance;
	}

public:
	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="soundHandle"></param>
	/// <param name="isLoop"></param>
	/// <param name="volume"></param>
	void PlayAudio(uint32_t soundHandle, bool isLoop, float volume);

	/// <summary>
	/// 名前から
	/// </summary>
	/// <param name="soundID"></param>
	/// <param name="isLoop"></param>
	/// <param name="volume"></param>
	void PlayAudio(const char* soundID, bool isLoop, float volume);

	/// <summary>
	/// タグから音源を取得
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	static uint32_t GetSound(const char* soundID);

	/// <summary>
	/// サウンド追加
	/// </summary>
	/// <param name="soundHandle"></param>
	/// <param name="soundID"></param>
	void AddSound(uint32_t soundHandle, const char* soundID);

private:
	/// <summary>
	/// サウンドリスト
	/// </summary>
	static std::vector<std::pair<char*, uint32_t>> soundLists_;

	/// <summary>
	/// Audioインスタンス
	/// </summary>
	Audio* audio_ = nullptr;

};

