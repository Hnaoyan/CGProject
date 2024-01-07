#include "ParticleData.h"
#include "MathCalc.h"

void ParticleData::UpdateMatrix()
{
	// ビルボードありの場合
	if (isBillBoard_) {
		Matrix4x4 billBoardMat = MatLib::MakeBillBoard(
			translation_, viewProjection_->translate_, Vector3(0.0f, 1.0f, 0.0f));

		Matrix4x4 worldMat = MatLib::MakeAffineMatrix(
			scale_, rotation_, translation_);
		matWorld_ = MatLib::Multiply(billBoardMat, worldMat);
	}
	// ビルボードなしの場合
	else {
		this->matWorld_ =
			MatLib::MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);
	}

	//親があれば親のワールド行列を掛ける
	//if (parent_) {
	//	matWorld_ = MatLib::Multiply(matWorld_, parent_->matWorld_);
	//}

}
