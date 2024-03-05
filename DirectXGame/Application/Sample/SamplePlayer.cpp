#include "SamplePlayer.h"
#include "imgui.h"

void SamplePlayer::Initialize(Model* model)
{
	input_ = Input::GetInstance();
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

}

void SamplePlayer::Update()
{

	ImGui::Begin("target");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.01f, -100, 100);
	ImGui::End();

	worldTransform_.UpdateMatrix();

}

void SamplePlayer::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}
