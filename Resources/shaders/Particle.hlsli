
cbuffer WorldTransform : register(b0)
{
    matrix world;
};


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

static const int DIRECTLIGHT_NUM = 3;


struct DirectLight
{
    float3 lightv;
    float3 lightcolor;
    uint active;
};

cbuffer LightGroup : register(b3)
{
    float3 ambientColor;
    DirectLight directLight[DIRECTLIGHT_NUM];
}



struct VSOutput
{
    float4 svPos : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};