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
            //float3 dotLightNormal = dot(directLight[i].lightv, input.normal);
            
            //float3 reflect = normalize(-directLight[i].lightv + 2 * dotLightNormal * input.normal);
            
            //float3 diffuse = dotLightNormal * m_diffuse;
            
            //float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            //shadecolor.rgb += (diffuse + specular) * directLight[i].lightcolor;
            
            //float3 lightDir = normalize(directLight[i].lightv);
            //float3 normal = normalize(input.normal);
            //float3 viewDir = normalize(cameraPos - input.worldPos.xyz);

            //float cosTheta = saturate(dot(normal, -lightDir)); // ライトと法線のなす角のcos値

            //// ランバートの拡散反射
            //float3 diffuse = m_diffuse * directLight[i].lightcolor * cosTheta;

            //// ブリン・フォンの鏡面反射
            //float3 halfwayDir = normalize(-lightDir + viewDir);
            //float specularStrength = pow(saturate(dot(normal, halfwayDir)), shininess);
            //float3 specular = m_specular * directLight[i].lightcolor * specularStrength;

            //shadecolor.rgb += (diffuse + specular); // ランバートとブリン・フォンの反射を合算
            float3 lightDir = normalize(directLight[i].lightv);
            float3 normal = normalize(input.normal);
            float3 viewDir = normalize(cameraPos - input.worldPos.xyz);

            float halfLambert = sqrt(max(0.5f * (1.0f + saturate(dot(normal, -lightDir))), 0.0f)); // Half Lambert

            float cosTheta = saturate(dot(normal, -lightDir)); // ライトと法線のなす角のcos値
            float3 halfwayDir = normalize(-lightDir + viewDir);
            float specularStrength = pow(saturate(dot(normal, halfwayDir)), shininess);

            float3 diffuse = m_diffuse * directLight[i].lightcolor * halfLambert; // Half Lambertを使ったディフューズ反射
            float3 specular = m_specular * directLight[i].lightcolor * specularStrength; // スペキュラー反射

            shadecolor.rgb += (diffuse + specular); // ディフューズとスペキュラーを合算
        }
    }
	
    return shadecolor * texcolor;
}