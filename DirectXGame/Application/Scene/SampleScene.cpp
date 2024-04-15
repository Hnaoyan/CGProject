#include "SampleScene.h"
#include "Model.h"
#include "Sprite.h"
#include "imgui.h"

SampleScene::SampleScene()
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	IScene::Initialize();

}

void SampleScene::Update()
{

	ImGuiUpdate();

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


}

void SampleScene::ImGuiUpdate()
{

}
