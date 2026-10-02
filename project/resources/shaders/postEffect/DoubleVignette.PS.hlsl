#include "FullScreen.hlsli"

struct Material
{
    float32_t radius;
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    float32_t2 uvLeft = input.texcoord - float32_t2(0.3f, 0.5f);
    float32_t2 uvRight = input.texcoord - float32_t2(0.7f, 0.5f);
    
    uvLeft.x *= 1280.0f / 720.0f;
    uvRight.x *= 1280.0f / 720.0f;
    
    float vignette = 0.0f;
    
    if (gMaterial.radius > 0.0f)
    {
        float vignetteLeft = saturate(1.0f - length(uvLeft) / gMaterial.radius);
        float vignetteRight = saturate(1.0f - length(uvRight) / gMaterial.radius);
        
        vignette = max(vignetteLeft, vignetteRight);
    }
    
    output.color.rgb *= vignette;
    return output;
}