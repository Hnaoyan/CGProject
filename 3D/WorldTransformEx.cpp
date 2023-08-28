#include "WorldTransform.h"
#include "MatrixMath.h"

void WorldTransform::UpdateMatrix() {
	this->matWorld_ =
	    MatrixMath::MakeAffineMatrix(this->scale_, this->rotation_, this->translation_);

	// 定数バッファに転送
	TransferMatrix();
}