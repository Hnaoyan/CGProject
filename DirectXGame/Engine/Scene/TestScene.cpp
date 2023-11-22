#include "TestScene.h"
#include "imgui.h"

void TestScene::Initialize()
{
}

void TestScene::Update()
{
	Matrix4x4 test = {};
	this->ImGuiMatrixPrintf(test,"zero");

	Matrix4x4 tes2 = MatLib::MakeIdentity4x4();
	this->ImGuiMatrixPrintf(tes2,"Identity");

}

void TestScene::Draw()
{
}

void TestScene::ImGuiMatrixPrintf(const Matrix4x4& matrix, const char* tag)
{

	ImGui::Begin("matrix");
	if (ImGui::TreeNode(tag)) {
		for (int y = 0; y < 4; y++) {
			ImGui::Text("%6.2f : %6.2f : %6.2f : %6.2f", matrix.m[y][0], matrix.m[y][1], matrix.m[y][2], matrix.m[y][3]);
		}
		ImGui::TreePop();
	}
	ImGui::End();

}
