#include "Object3d.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
    float32_t environmentCoefficient;
    int32_t useEnvironment;
};

struct ToyTexture
{
    float32_t rimLightPower;
    float32_t rimLightIntensity;
    float32_t saturation;
    float32_t contrast;
    float32_t ambientStrength;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct Camera
{
    float32_t3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<ToyTexture> gToy : register(b3);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
    
    float NdotH = dot(normalize(input.normal), halfVector);
    float specularPow = pow(saturate(NdotH), gMaterial.shininess);
    
    
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    if (gMaterial.enableLighting != 0)
    {
       
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        if (cos > 0.8f)
        {
            cos = 1.0f;
        }
        else if (cos > 0.5f)
        {
            cos = 0.7f;
        }
        else
        {
            cos = 0.35f;
        }
        
        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        float32_t3 specular = gMaterial.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        output.color.rgb = diffuse + specular;
        
        float32_t rim = 1.0f - saturate(dot(input.normal, toEye));
        rim = pow(rim, gToy.rimLightPower);
        output.color.rgb += rim * gToy.rimLightIntensity;
        
        float32_t gray = dot(output.color.rgb, float3(0.2125f, 0.7154f, 0.0721f));
        output.color.rgb = lerp(gray.xxx, output.color.rgb, gToy.saturation);
        
        output.color.rgb = (output.color.rgb - 0.5f) * gToy.contrast + 0.5f;
        
        float32_t frensel = pow(1.0f - saturate(dot(input.normal, toEye)), 5.0f);
        output.color.rgb += frensel * 0.2f;
        
        output.color.rgb += gToy.ambientStrength;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    
    return output;
}