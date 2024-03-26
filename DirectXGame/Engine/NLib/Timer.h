#pragma once
class Timer
{
public:
	Timer() {};
	~Timer() = default;

public:
	/// <summary>
	/// タイマー設定関数
	/// </summary>
	/// <param name="endFrame"></param>
	void Start(float endFrame = 60.0f);

	/// <summary>
	/// タイマー終了関数
	/// </summary>
	void End();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public: // アクセッサ

	bool IsEnd() { return isEnd_; }
	bool IsActive() { return isActive_; }

	float GetNowFrame() { return nowFrame_; }

private:
	/// <summary>
	/// 初期化関数
	/// </summary>
	void StartSetting();

private:
	// 終了フレーム
	float endFrame_;
	// 現在のフレーム
	float nowFrame_;
	// 終了フラグ
	bool isEnd_;
	// 有効フラグ
	bool isActive_;

};

