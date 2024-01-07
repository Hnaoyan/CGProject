#include "Particle.hlsli"

Texture2D<float4> tex : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
	
    
    return texcolor;
}