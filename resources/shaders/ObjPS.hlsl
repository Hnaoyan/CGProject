#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv);
	
    const float shininess = 4.0f;
	
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
	
    float3 ambient = m_ambient;
	
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	
    for (int i = 0; i < DIRECTLIGHT_NUM; i++)
    {
        if (directLight[i].active)
        {
            float3 dotLightNormal = dot(directLight[i].lightv, input.normal);
            
            float3 reflect = normalize(-directLight[i].lightv + 2 * dotLightNormal * input.normal);
            
            float3 diffuse = dotLightNormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * directLight[i].lightcolor;
        }
    }
	
	
        return shadecolor * texcolor;
}