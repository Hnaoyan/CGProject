#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint32_t instanceId : SV_InstanceID)
{
	//
    float4 worldNormal = normalize(mul(float4(normal, 0), gWorldTransform[instanceId].world));
    float4 worldPos = mul(pos, gWorldTransform[instanceId].world);
	
    VSOutput output;
    output.svPos = mul(pos, mul(gWorldTransform[instanceId].world, mul(view, projection)));
    
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
    return output;
}