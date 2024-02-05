#pragma once
#include "IScene.h"
#include "Drawer/Instancing.h"

class SampleScene : public IScene
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	SampleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SampleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	Instancing* inst_;

};

