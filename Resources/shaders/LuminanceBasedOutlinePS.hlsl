#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const float32_t2 kIndex3x3[3][3] =
{
    {
        { -1.0f, -1.0f },
        { 0.0f, -1.0f },
        { 1.0f, -1.0f }
    },
    {
        { -1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f }
    },
    {
        { -1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
    },
};

static const float32_t2 kIndex5x5[5][5] =
{
    { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
    { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
    { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
    { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
    { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } },
};

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height; // uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    int32_t kernelSize = 5;
    
    float kernelSizeSquared = 1.0f / (kernelSize * kernelSize);
    
    for (int32_t x = 0; x < kernelSize; ++x)
    {
        for (int32_t y = 0; y < kernelSize; ++y)
        {
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex5x5[x][y] * uvStepSize;
            // 4. 色に1/9掛けて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            
            // 計算
            output.color.rgb += fetchColor * kernelSizeSquared;
        }
    }
    
    return output;
}
