struct Particle
{
    matrix world;
    float3 velocity;
};

StructuredBuffer<Particle> gTransform : register(t1);

cbuffer ViewProjection : register(b1)
{
    matrix view;
    matrix projection;
    float3 cameraPos;
};


cbuffer Material : register(b2)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
}


struct VSOutput
{
    float4 svPos : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    //uint id : SV_InstanceID;
};