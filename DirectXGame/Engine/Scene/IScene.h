#pragma once
#include "DirectXCommon.h"
#include "Audio.h"
#include "Input.h"
#include "ViewProjection.h"

class IScene 
{
public: // 仮想関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
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
	virtual ~IScene() = default;

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


	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;


};
