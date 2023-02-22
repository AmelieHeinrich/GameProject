/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 17:52
 */

struct RendererSettings
{
    int Tonemapper;

    float Exposure;
    float Temperature;
    float Tint;
    float3 Contrast;
    float3 LinearMidPoint;
    float3 Brightness;
    float3 ColorFilter;
    float ColorFilterIntensity;
    float3 Saturation;

    float SharpnessStrength;

    float pad[6];
};

RWTexture2D<float4> Texture : register(u0);
ConstantBuffer<RendererSettings> Settings : register(b1);

[numthreads(16, 16, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    uint Width, Height;
    Texture.GetDimensions(Width, Height);

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        float2 Offsets[9] = {
            float2(0.0f, 0.0f),
            float2(0.0f, 1.0f),
            float2(0.0f, -1.0f),  
            float2(1.0f, 0.0f),
            float2(-1.0f, 0.0f),
            float2(-1.0f, 1.0f),
            float2(-1.0f, -1.0f),
            float2(1.0f, -1.0f),
            float2(1.0f, -1.0f)
        };

        float Kernel[9] = {
            1.0f / 16.0f , 2.0f / 16.0f, 1.0f / 16.0f,
            2.0f / 16.0f , 4.0f / 16.0f, 2.0f / 16.0f,
            1.0f / 16.0f , 2.0f / 16.0f, 1.0f / 16.0f
        };

        float3 Color = float3(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 9; i++) {
            Color += Texture[ThreadID.xy + Offsets[i]].rgb * Kernel[i] * max(1.0f, Settings.SharpnessStrength);
        }

        Texture[ThreadID.xy] = float4(Color, 1.0);
    }
}
