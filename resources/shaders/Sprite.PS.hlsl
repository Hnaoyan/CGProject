#include "Sprite.hlsli"

Texture2D<float32_t4> tex : register(t0);
SamplerState smp : register(s0);

float32_t4 main(VSOutput input) : SV_TARGET{
	float32_t4 textureColor = tex.Sample(smp,input.uv) * color;
    if (textureColor.a <= 0.01f)
    {
        discard;
    }
	return textureColor;
}