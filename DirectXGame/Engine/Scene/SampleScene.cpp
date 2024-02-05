#include "SampleScene.h"
#include "Model.h"
#include "Sprite.h"

SampleScene::SampleScene()
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	IScene::Initialize();
	//inst_ = new Instancing();
	//inst_->Create();
	//inst_->Initialize();
}

void SampleScene::Update()
{

	//inst_->Update();

	// 更新
	viewProjection_.UpdateMatrix();
}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// 描画前処理
	Sprite::PreDraw(commandList);


	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();


	// 描画前処理
	Model::PreDraw(commandList);


	// 描画後処理
	Model::PostDraw();

	//inst_->PreDraw(commandList);
	//inst_->Draw();
	//inst_->PostDraw();

}
