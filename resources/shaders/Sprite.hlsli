cbuffer cbuff0 : register(b0) {
	float32_t4 color;
	float32_t4x4 mat;
}

struct VSOutput {
	float32_t4 svPos : SV_POSITION;
	float32_t2 uv : TEXCOORD;
};