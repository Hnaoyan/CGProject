#include "Particle.hlsli"

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instanceId : SV_InstanceID)
//{
//    float4 worldNormal = normalize(mul(float4(normal, 0), gTransform[instanceId].world));
//    float4 worldPos = mul(pos, gTransform[instanceId].world);
    
//    VSOutput output;
//    output.svPos = mul(pos, mul(gTransform[instanceId].world, mul(view, projection)));
//    output.worldPos = worldPos;
//    output.normal = worldNormal.xyz;
//    output.uv = uv;
	    
//    return output;
//}

StructuredBuffer<ParticleForGPU> gParticle : register(t0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);
VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    float32_t4x4 WorldViewProjection = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
    output.position = mul(input.position, mul(gParticle[instanceId].matWorld, WorldViewProjection));
    output.texcoord = input.texcoord;
	//output.normal = normalize(mul(input.normal, (float32_t3x3)WorldViewProjection));
    output.color = gParticle[instanceId].color;
    return output;

}