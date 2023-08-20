#include "Sprite.hlsli"

VertexShaderOutput main(float32_t4 position : POSITION,
	float32_t2 texcoord : TEXCOORD, float32_t3 normal : NORMAL) {
	VertexShaderOutput output;
	output.position = mul(position, gTransformationMatrix.WVP);
	output.texcoord = texcoord;
	output.normal = normalize(mul(normal, (float32_t3x3)gTransformationMatrix.World));
	return output;
}