//struct Particle
//{
//    matrix world;
//    float3 velocity;
//};

//StructuredBuffer<Particle> gTransform : register(t1);

//cbuffer ViewProjection : register(b1)
//{
//    matrix view;
//    matrix projection;
//    float3 cameraPos;
//};


//cbuffer Material : register(b2)
//{
//    float3 m_ambient : packoffset(c0);
//    float3 m_diffuse : packoffset(c1);
//    float3 m_specular : packoffset(c2);
//    float m_alpha : packoffset(c2.w);
//}


//struct VSOutput
//{
//    float4 svPos : SV_POSITION;
//    float4 worldPos : POSITION;
//    float3 normal : NORMAL;
//    float2 uv : TEXCOORD;
//    //uint id : SV_InstanceID;
//};
struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
    float32_t4 color : COLOR0;
};
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};
struct TransformationMatrix
{

    float32_t4x4 matWorld;
};
struct ViewProjectionMatrix
{
    float32_t4x4 view;
    float32_t4x4 projection;
    float32_t3 camera;
};
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};
struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
struct ParticleForGPU
{
    float32_t4x4 matWorld;
    float32_t4 color;
};