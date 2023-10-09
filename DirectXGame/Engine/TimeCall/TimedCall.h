#pragma once
#include <functional>

class TimedCall
{

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="function"></param>
	/// <param name="time"></param>
	TimedCall(std::function<void()> function, uint32_t time);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 完了ならtrueを返す
	/// </summary>
	/// <returns>true</returns>
	bool IsFinished() { return true; }

private: //メンバ変数
	//コールバック
	std::function<void()> function_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinished = false;

};

