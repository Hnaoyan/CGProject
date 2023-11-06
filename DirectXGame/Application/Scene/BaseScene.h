#pragma once
#include "SceneManager.h"

class BaseScene 
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(){};
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(){};
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(){};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseScene() = default;

public: // 取得・設定
	/// <summary>
	/// シーン番号取得
	/// </summary>
	/// <returns></returns>
	int GetSceneNum() { return sceneNum; }

protected:
	/// <summary>
	/// シーン番号
	/// </summary>
	static int sceneNum;
};
