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
	inst_->Initialize(&viewProjection_);

	plWTF_.Initialize();
	testModel_.reset(Model::Create());

	assimpTest_.reset(Model::Create());

	assimpTrf.Initialize();
	for (uint32_t i = 0; i < 5; ++i) {
		assimpTrf.translation_ = { 0,-10.0f + (i * 2.0f),(i * 4.0f) };
		trfLists_.push_back(assimpTrf);
	}

	assimpTrf.translation_ = { 5.0f,-2.0f,10.0f };

	target_ = std::make_unique<SamplePlayer>();
	target_->Initialize(testModel_.get());
	target_->InitSetting({ 10.0f,0,100.0f });
	texture_[0] = TextureManager::Load("uvChecker.png");
	texture_[1] = TextureManager::Load("white1x1.png");

	emitter_ = std::make_unique<IEmitter>();
	emitter_->Initialize(1, 30);
	emitter_->transform.translate = newPoint;

	sprite_.reset(Sprite::Create(texture_[0], { 200,200 }, { 1,1,1,1 }, { 0.5f,0.5f }, false, false));

	//for (std::list<std::unique_ptr<Sprite>>::iterator it = sprites_.begin();
	//	it != sprites_.end(); ++it) {
	//	(*it).reset(Sprite::Create(texture_[0], { 200,200 }, { 1,1,1,1 }, { 0.5f,0.5f }, false, false));
	//}

}

void SampleScene::Update()
{
	ImGuiUpdate();

	//inst_->Update();
	assimpTrf.UpdateMatrix();
	target_->Update();
	emitter_->Update(newPoint);

	for (WorldTransform obj : trfLists_) {
		obj.UpdateMatrix();
	}

	for (SamplePlayer* obj : targetObjs_) {
		obj->Update();
	}
	//for (std::list<std::unique_ptr<Sprite>>::iterator it = sprites_.begin();
	//	it != sprites_.end(); ++it) {
	//	(*it)->Update();
	//}
	sprite_->Update();
	// 更新
	viewProjection_.UpdateMatrix();
}

void SampleScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 描画前処理
	Sprite::PreDraw(commandList);

	//for (std::list<std::unique_ptr<Sprite>>::iterator it = sprites_.begin();
	//	it != sprites_.end(); ++it) {
	//	(*it)->Draw();
	//}
	sprite_->Draw();
	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


	// 描画前処理
	Model::PreDraw(commandList);

	//target_->Draw(viewProjection_);
	for (WorldTransform obj : trfLists_) {
		//assimpTest_->Draw(obj, viewProjection_);
		testModel_->Draw(obj, viewProjection_);
	}

	//assimpTest_->Draw(assimpTrf, viewProjection_);

	//for (SamplePlayer* obj : targetObjs_) {
	//	obj->Draw(viewProjection_);
	//}

	// 描画後処理
	Model::PostDraw();

	Instancing::PreDraw(commandList);
	inst_->Draw(texture_[1]);
	Instancing::PostDraw();

}

void SampleScene::ImGuiUpdate()
{

	ImGui::Begin("sampleScene");

	ImGui::DragFloat3("RegisterPosition", &newPoint.x, 0.01f, -100.0f, 100.0f);

	if (ImGui::Button("RegisterObject")) {
		RegisterList(newPoint);
	}

	if (ImGui::Button("CreateEmitter")) {


		inst_->AddEmitter(emitter_.get());
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

#pragma region ImGui用関数
void SampleScene::ImGuiMatrixPrintf(const Matrix4x4& matrix, const char* tag)
{

	ImGui::Begin("4x4Matrix");
	if (ImGui::TreeNode(tag)) {
		for (int y = 0; y < 4; y++) {
			ImGui::Text("%6.3f : %6.3f : %6.3f : %6.3f", matrix.m[y][0], matrix.m[y][1], matrix.m[y][2], matrix.m[y][3]);
		}
		ImGui::TreePop();
	}
	ImGui::End();

}

void SampleScene::ImGuiVector3Printf(const Vector3& vector, const char* tag)
{
	ImGui::Begin("Vector3");
	if (ImGui::TreeNode(tag)) {
		ImGui::Text("%6.3f : %6.3f : %6.3f : %s", vector.x, vector.y, vector.z, tag);
		ImGui::TreePop();
	}
	ImGui::End();

}

void SampleScene::ImGuiQuaternionPrintf(const Quaternion& quat, const char* tag)
{
	ImGui::Begin("Quaternion");
	if (ImGui::TreeNode(tag)) {
		ImGui::Text("%6.3f : %6.3f : %6.3f : %6.3f : %s", quat.x, quat.y, quat.z, quat.w, tag);
		ImGui::TreePop();
	}
	ImGui::End();
}

#pragma endregion
