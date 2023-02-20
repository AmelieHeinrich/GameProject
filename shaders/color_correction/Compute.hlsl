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
};

Texture2D<float4> HDRTexture : register(t0);
RWTexture2D<float4> LDRTexture : register(u1);

[numthreads(16, 16, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    uint Width, Height;
    HDRTexture.GetDimensions(Width, Height);

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        LDRTexture[ThreadID.xy] = HDRTexture[ThreadID.xy];
    }
}
