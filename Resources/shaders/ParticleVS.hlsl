#include "Particle.hlsli"

struct Particle
{
    matrix world;
    float3 velocity;
};

StructuredBuffer<Particle> gTransform : register(t0);
//ConstantBuffer<WorldTransform> gTransform : register(b0);

//struct ParticleGPU
//{
//    matrix WVP;
//    matrix World;
//   // float32_t4 color;
//};

//StructuredBuffer<ParticleGPU> gParticle : register(t1);

//struct VertexShaderInput
//{
//    matrix 
//};



VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint32_t instanceId : SV_InstanceID)
{
    float4 worldNormal = normalize(mul(float4(normal, 0), gTransform[instanceId].world));
    float4 worldPos = mul(pos, gTransform[instanceId].world);
    
    VSOutput output;
    output.svPos = mul(pos, mul(gTransform[instanceId].world, mul(view, projection)));
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
    //output.svPos = float4(pos);
    //output.worldPos = pos;
    //output.normal = normal;
    //output.uv = uv;
    
    return output;
}