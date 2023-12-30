#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    WorldTransform worldTest;
	//
    float4 worldNormal = normalize(mul(float4(normal, 0), worldTest.world));
    float4 worldPos = mul(pos, worldTest.world);
    
    VSOutput output;
    output.svPos = mul(pos, mul(worldTest.world, mul(view, projection)));
    
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
    return output;
}