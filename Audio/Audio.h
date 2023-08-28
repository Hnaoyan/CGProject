#pragma once
#include <wrl.h>
#include <array>
#include <set>
#include <string>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class Audio
{
public:
	// サウンドデータの最大数
	static const int kMaxSoundData = 256;

	// チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];	// チャンク毎のID
		int32_t size;	// チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	// "RIFF"
		char type[4];
	};

	// FMTチャンク
	struct FormaChunk
	{
		ChunkHeader chunk;	// "fmt"
		WAVEFORMATEX fmt;	// 波形フォーマット
	};

	// 音声データ
	struct SoundData
	{
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* pBuffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 名前
		std::string name_;
	};

	// 再生データ
	struct Voice {
		uint32_t handle = 0u;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

public:
	/// <summary>
	/// シングルトンインスタンス
	/// </summary>
	/// <returns></returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& directoryPath = "resources/music/");

	/// <summary>
	/// wavファイルの読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadWave(const std::string& fileName);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// wavの再生
	/// </summary>
	/// <param name="soundHandle"></param>
	/// <param name="loopFlag"></param>
	/// <param name="volume"></param>
	/// <returns></returns>
	uint32_t PlayWave(uint32_t soundHandle, bool loopFlag = false, float volume = 1.0f);

	/// <summary>
	/// 再生中の音源を停止
	/// </summary>
	/// <param name="voiceHandle"></param>
	void StopWave(uint32_t voiceHandle);

	/// <summary>
	/// 再生中かどうかの判断
	/// </summary>
	/// <param name="voiceHandle"></param>
	/// <returns></returns>
	bool IsPlaying(uint32_t voiceHandle);
	
	/// <summary>
	/// 音声のサイズ設定
	/// </summary>
	/// <param name="voiceHandle"></param>
	/// <param name="volume"></param>
	void SetVolume(uint32_t voiceHandle, float volume);

	/// <summary>
	/// 終了時の処理
	/// </summary>
	void Finalize();

private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&&) = delete;

	// XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	
	// コンテナ系
	// サウンドデータコンテナ
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// 再生中データコンテナ
	std::set<Voice*> voices_;

	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0u;
	// 次に使う再生中データの番号
	uint32_t indexVoice_ = 0u;

};

