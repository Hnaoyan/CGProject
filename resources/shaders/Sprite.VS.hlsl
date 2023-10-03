#include "Sprite.hlsli"

VSOutput main(float32_t4 pos : POSITION, float32_t2 uv : TEXCOORD) {
	VSOutput output;
	output.svPos = mul(pos, mat);
	output.uv = uv;
	return output;
}