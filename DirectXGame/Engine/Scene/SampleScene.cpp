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
	inst_ = new Instancing();
	inst_->Create();
	inst_->Initialize();

	plWTF_.Initialize();
	testModel_.reset(Model::Create());

	target_ = std::make_unique<SamplePlayer>();
	target_->Initialize(testModel_.get());
	target_->InitSetting({ 10.0f,0,100.0f });

}

void SampleScene::Update()
{

	ImGuiUpdate();

	//inst_->Update();
	target_->Update();

	for (SamplePlayer* obj : targetObjs_) {
		obj->Update();
	}

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

	target_->Draw(viewProjection_);

	for (SamplePlayer* obj : targetObjs_) {
		obj->Draw(viewProjection_);
	}

	// 描画後処理
	Model::PostDraw();

	inst_->PreDraw(commandList);
	//inst_->Draw();
	inst_->PostDraw();

}

void SampleScene::ImGuiUpdate()
{

	ImGui::Begin("sampleScene");

	ImGui::DragFloat3("RegisterPosition", &newPoint.x, 0.01f, -100.0f, 100.0f);

	if (ImGui::Button("RegisterObject")) {
		RegisterList(newPoint);
	}

	ImGui::End();

}

void SampleScene::RegisterList(const Vector3& position)
{
	SamplePlayer* newInstance = new SamplePlayer();
	newInstance->Initialize(testModel_.get());
	newInstance->InitSetting(position);

	this->targetObjs_.push_back(newInstance);

}
