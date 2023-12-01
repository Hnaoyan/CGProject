#include "TestScene.h"
#include "imgui.h"
#include "DirectXCommon.h"

void TestScene::Initialize()
{
	audio_ = AudioManager::GetInstance();

	testTransform_.Initialize();
	view_.Initialize();

	testModel_.reset(Model::Create());

}

void TestScene::Update()
{
	Quaternion rotation =
		QuatLib::MakeRotateAxisAngleQuaternion(MathCalc::Normalize({ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };

	Matrix4x4 rotateMat = MatLib::MakeRotateMatrix(rotation);

	Vector3 rotateByQuat = MatLib::RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = MatLib::Transform(pointY, rotateMat);

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

	testTransform_.ImGuiWidget();
	testTransform_.UpdateMatrix();
	view_.ImGuiWidget();
	view_.UpdateMatrix();

}

void TestScene::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 描画前処理
	Model::PreDraw(commandList);

	testModel_->Draw(testTransform_, view_);

	// 描画後処理
	Model::PostDraw();
#pragma endregion

}

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
