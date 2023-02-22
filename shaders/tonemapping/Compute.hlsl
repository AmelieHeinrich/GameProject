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

float3 ACESFilm(float3 X)
{
    float A = 2.51f;
    float B = 0.03f;
    float C = 2.43f;
    float D = 0.59f;
    float E = 0.14f;
    return saturate((X * (A * X + B)) / (X * (C * X + D) + E));
}

float3 Filmic(float3 X)
{
    X = max(float3(0.0, 0.0, 0.0), X - float3(0.004, 0.004, 0.004));
    X = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
    return X;
}

float3 RomBinDaHouse(float3 X)
{
    X = exp(-1.0 / (2.72 * X + 0.15));
	return X;
}

Texture2D<float4> HDRTexture : register(t0);
RWTexture2D<float4> LDRTexture : register(u1);
ConstantBuffer<RendererSettings> Settings : register(b2);

[numthreads(16, 16, 1)]
void CSMain(uint3 ThreadID : SV_DispatchThreadID)
{
    float4 HDRColor = HDRTexture[ThreadID.xy];
    float3 MappedColor = HDRColor.xyz;
    switch (Settings.Tonemapper)
    {
        case 0:
            MappedColor = ACESFilm(MappedColor);
            break;
        case 1:
            MappedColor = Filmic(MappedColor);
            break;
        case 2:
            MappedColor = RomBinDaHouse(MappedColor);
            break;
    }
    
    LDRTexture[ThreadID.xy] = float4(MappedColor, HDRColor.a);
}
