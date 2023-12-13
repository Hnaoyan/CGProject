#pragma once

struct Vector4 final{
	float x, y, z, w;


#pragma region 四則演算

	// 加算減算
	Vector4 operator+(const Vector4& Other) const {
		return Vector4{ this->x + Other.x, this->y + Other.y, this->z + Other.z, this->w + Other.w };
	}
	Vector4 operator-(const Vector4& Other) const {
		return Vector4{ this->x - Other.x, this->y - Other.y, this->z - Other.z, this->w - Other.w};
	}

	Vector4& operator+=(const Vector4& Other) {
		this->x += Other.x;
		this->y += Other.y;
		this->z += Other.z;
		this->w += Other.w;
		return *this;
	}
	Vector4& operator-=(const Vector4& Other) {
		this->x -= Other.x;
		this->y -= Other.y;
		this->z -= Other.z;
		this->w -= Other.w;
		return *this;
	}

	// 乗算除算
	Vector4 operator*(const Vector4& Other) const {
		return Vector4{ this->x * Other.x, this->y * Other.y, this->z * Other.z, this->w * Other.w };
	}
	Vector4 operator/(const Vector4& Other) const {
		return Vector4{ this->x / Other.x, this->y / Other.y, this->z / Other.z, this->w / Other.w };
	}

	Vector4 operator*(const float& Scaler) const {
		return Vector4{ this->x * Scaler, this->y * Scaler, this->z * Scaler, this->w * Scaler };
	}
	Vector4 operator/(const float& Scaler) const {
		return Vector4{ this->x / Scaler, this->y / Scaler, this->z / Scaler, this->w / Scaler };
	}

	Vector4& operator*=(const float& Scaler) {
		this->x *= Scaler;
		this->y *= Scaler;
		this->z *= Scaler;
		this->w *= Scaler;
		return *this;
	}
	Vector4& operator/=(const float& Scaler) {
		this->x /= Scaler;
		this->y /= Scaler;
		this->z /= Scaler;
		this->w /= Scaler;
		return *this;
	}

#pragma endregion

};

