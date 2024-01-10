#include "TestScene.h"
#include "imgui.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Editor/Editor.h"
#include "MathCalc.h"

void TestScene::Initialize()
{
	audio_ = AudioManager::GetInstance();
	input_ = Input::GetInstance();

	testTransform_.Initialize();
	objTransform_.Initialize();
	view_.Initialize();

	testModel_.reset(Model::Create());
	obj2Model_.reset(Model::Create());
	planeModel_.reset(Model::CreatePlane());

	texture_ = TextureManager::Load("Texture/Circle.png");

	texture_ = TextureManager::Load("uvChecker.png");
	testSprite_.reset(Sprite::Create(texture_, { 100,100 }, { 1,1,1,1 }, { 0.5f,0.5f }, 0, 0));

	alphaValue_ = 1.0f;

	Editor* editor = Editor::GetInstance();
	Editor::HierarchicalName nameGroup;
	nameGroup.kGroup = "First";
	nameGroup.kSection = "Alpha";

	editor->CreateHierarchy(nameGroup);
	editor->AddItem(nameGroup, "one", testValue_);
	editor->AddItem(nameGroup, "two", testValue_);
	editor->AddItem(nameGroup, "three", this->editTest);
	nameGroup.kSection = "Four";
	editor->AddItem(nameGroup, "one", editTest);


	enemyInfo1_ = { {1,1,1},0.1f,10 };
	//editor->AddItem(nameGroup, "three", enemyInfo1_);

	nameGroup.kGroup = "Second";
	editor->CreateHierarchy(nameGroup);
	editor->AddItem(nameGroup, "one", testValue_);

	nameGroup = { "Third","Alpha" };
	editor->CreateHierarchy(nameGroup);
	nameGroup = { "Third","Beta" };
	editor->CreateHierarchy(nameGroup);

	ApplyGlobalVariables();

}

void TestScene::ApplyGlobalVariables()
{
	Editor* editor = Editor::GetInstance();
	Editor::HierarchicalName nameGroup;
	nameGroup.kGroup = "First";
	nameGroup.kSection = "Alpha";

	testValue_ = editor->GetIntValue(nameGroup, "one");
	//enemyInfo1_ = editor->GetEnemyInfoValue(nameGroup, "three");
	//editTest = editor->GetFloatValue(nameGroup, "one");
}

void TestScene::Update()
{
	Quaternion rotation =
		QuatLib::MakeRotateAxisAngleQuaternion(MathCalc::Normalize({ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };

	Matrix4x4 rotateMat = MatLib::MakeRotateMatrix(rotation);

	Vector3 rotateByQuat = MatLib::RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = MatLib::Transform(pointY, rotateMat);

	GetMousePosition();

	ImGui::Begin("Check");
	ImGui::Text("Alpha:One : %d", TexValue);
	static char newChar[256];
	ImGui::InputText("New Group", newChar, IM_ARRAYSIZE(newChar));
	if (ImGui::Button("Save")) {
		std::string newString = newChar;
		Editor::HierarchicalName CreateName = { "First",newString };
		Editor::GetInstance()->CreateHierarchy(CreateName);
		Editor::GetInstance()->SaveFile(CreateName);
	}
	if (ImGui::Button("SaveKey")) {
		std::string newKey = newChar;



	}
	ImGui::End();

	this->ImGuiMatrixPrintf(rotateMat,"zero");
	this->ImGuiVector3Printf(rotateByQuat,"Quat");
	this->ImGuiVector3Printf(rotateByMatrix, "Matrix");
	this->ImGuiQuaternionPrintf(rotation, "test");


	Vector3 a = { 1,1,1 };
	Vector3 b = { 2,2,2 };

	Vector3 Add_ab = a + b;
	Vector3 Sub_ab = a - b;
	Vector3 Mul_ab = a * b;
	Vector3 Div_ab = a / b;

	this->ImGuiVector3Printf(Add_ab, "Add");
	this->ImGuiVector3Printf(Sub_ab, "Sub");
	this->ImGuiVector3Printf(Mul_ab, "Mul");
	this->ImGuiVector3Printf(Div_ab, "Div");

	testTransform_.ImGuiWidget("test");
	testTransform_.UpdateMatrix();
	objTransform_.ImGuiWidget("obj");
	objTransform_.translation_ = savePoint_;
	objTransform_.UpdateMatrix();
	view_.ImGuiWidget();
	view_.UpdateMatrix();

	ImGui::Begin("model");
	ImGui::DragFloat("alpha", &alphaValue_, 0.01f, 0, 1.0f);
	ImGui::End();

	planeModel_->SetAlphaValue(alphaValue_);

	ImGui::ShowDemoWindow();
	ApplyGlobalVariables();
}

void TestScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

#pragma region 背景スプライト描画
	// 描画前処理
	Sprite::PreDraw(commandList);

	testSprite_->Draw();

	// 描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 描画前処理
	Model::PreDraw(commandList);

	testModel_->Draw(testTransform_, view_);
	obj2Model_->Draw(objTransform_, view_);

	planeModel_->Draw(objTransform_, view_, texture_);

	// 描画後処理
	Model::PostDraw();
#pragma endregion

}

#pragma region ImGui用関数
void TestScene::ImGuiMatrixPrintf(const Matrix4x4& matrix, const char* tag)
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

void TestScene::ImGuiVector3Printf(const Vector3& vector, const char* tag)
{
	ImGui::Begin("Vector3");
	if (ImGui::TreeNode(tag)) {
		ImGui::Text("%6.3f : %6.3f : %6.3f : %s", vector.x, vector.y, vector.z, tag);
		ImGui::TreePop();
	}
	ImGui::End();

}

void TestScene::ImGuiQuaternionPrintf(const Quaternion& quat, const char* tag)
{
	ImGui::Begin("Quaternion");
	if (ImGui::TreeNode(tag)) {
		ImGui::Text("%6.3f : %6.3f : %6.3f : %6.3f : %s", quat.x, quat.y, quat.z, quat.w, tag);
		ImGui::TreePop();
	}
	ImGui::End();
}

void TestScene::GetMousePosition()
{
	POINT mousePos;

	GetCursorPos(&mousePos);

	HWND hwnd = WindowAPI::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);

	nowPoint_ = { (float)mousePos.x,(float)mousePos.y,0 };

	// ビューポート行列
	Matrix4x4 matViewport =
		MatLib::MakeViewportMatrix(0, 0, WindowAPI::kClientWidth, WindowAPI::kClientHeight, 0, 1);

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = MatLib::Multiply(
		MatLib::Multiply(view_.matView, view_.matProjection), matViewport);
	// 合成行列の逆行列を計算
	Matrix4x4 matInverseVPV = MatLib::MakeInverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(nowPoint_.x, nowPoint_.y, 0);
	Vector3 posFar = Vector3(nowPoint_.x, nowPoint_.y, 1);

	// スクリーン座標系からワールド座標系へ
	posNear = MatLib::Transform(posNear, matInverseVPV);
	posFar = MatLib::Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = VectorLib::Subtract(posFar, posNear);
	mouseDirection = MathCalc::Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	mouseDirection.x *= 50;
	mouseDirection.y *= 50;
	//mouseDirection.z *= 50;
	mouseDirection.z = 0;

	testTransform_.translation_ = mouseDirection;
	testTransform_.UpdateMatrix();

	if (input_->TriggerKey(DIK_SPACE)) {
		savePoint_ = mouseDirection;
	}

	if (input_->PressKey(DIK_LEFT)) {
		savePoint_.x -= 0.1f;
	}

	ImGui::Begin("Mouse");
	if (ImGui::TreeNode("point")) {
		ImGui::DragFloat3("now", &nowPoint_.x);
		ImGui::DragFloat3("save", &savePoint_.x);

		ImGui::TreePop();
	}

	ImGui::End();

}


#pragma endregion
