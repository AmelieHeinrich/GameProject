/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 17:52
 */

float3 ACESFilm(float3 X)
{
    float A = 2.51f;
    float B = 0.03f;
    float C = 2.43f;
    float D = 0.59f;
    float E = 0.14f;
    return saturate((X * (A * X + B)) / (X * (C * X + D) + E));
}

Texture2D<float4> HDRTexture : register(t0);
RWTexture2D<float4> LDRTexture : register(u1);

[numthreads(16, 16, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    uint Width, Height;
    HDRTexture.GetDimensions(Width, Height);

    if (ThreadID.x < Width && ThreadID.y < Height)
    {
        float4 HDRColor = HDRTexture[ThreadID.xy];
        LDRTexture[ThreadID.xy] = float4(ACESFilm(HDRColor.xyz), HDRColor.a);
    }
}
