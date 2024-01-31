#include "Object3d.hlsli"

struct Material {
	float32_t4 color;
	int32_t enableLighting;
    float32_t shininess;
};

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};

struct Camera
{
    float32_t3 worldPosition;
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

//PixelShaderOutput main(VertexShaderOutput input) {
//	PixelShaderOutput output;
//	float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
//	if (gMaterial.enableLighting != 0) {
//		// half lambert
//		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
//		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
//		//float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
//		output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
//	}
//	else {// Lightingしない場合。前回までと同じ演算
//		output.color = gMaterial.color * textureColor;
//	}
//	//output.color = gMaterial.color * textureColor;
//	return output;
//}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    //float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
	
	// 鏡面反射
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);
		// 拡散反射
        float32_t3 diffuse =
			gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		// 鏡面反射
        float32_t3 specular =
			gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		// 拡散反射+鏡面反射
        output.color.rgb = diffuse + specular;
		// α
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        //output.color = gMaterial.color * textureColor;
    }

	// textureかoutput.colorのα値が0の時にPixelを棄却
    if (textureColor.a == 0.0 || output.color.a == 0.0)
    {
        discard;
    }

    return output;
}
