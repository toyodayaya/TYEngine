#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    float32_t2 uvLeft = input.texcoord - float32_t2(0.35f, 0.5f);
    float32_t2 uvRight = input.texcoord - float32_t2(0.65f, 0.5f);
    
    uvLeft.x *= 1280.0f / 720.0f;
    uvRight.x *= 1280.0f / 720.0f;
    
    float vignetteLeft = saturate(1.0f - length(uvLeft) / 0.6f);
    float vignetteRight = saturate(1.0f - length(uvRight) / 0.6f);

    
    float vignette = max(vignetteLeft, vignetteRight);
    output.color.rgb *= vignette;
    return output;
}