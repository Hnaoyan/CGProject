#include "Particle.hlsli"

//Texture2D<float4> tex : register(t0);
//SamplerState smp : register(s0);

//float4 main(VSOutput input) : SV_TARGET
//{
//    float4 texcolor = tex.Sample(smp, input.uv);
//    if (texcolor.a == 0.0)
//    {
//        discard;
//    }
    
//    return texcolor;
//}
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t1);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);



    output.color = gMaterial.color * textureColor * input.color;

    return output;
}