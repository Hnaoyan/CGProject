#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD) {
	VSOutput output;
	output.svPos = mul(pos, mat);
	output.uv = uv;
	return output;
}