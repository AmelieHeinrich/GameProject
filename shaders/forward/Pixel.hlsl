/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 16:39
 */

struct FragmentIn
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TextureCoords: TEXCOORD;
};

Texture2D Texture : register(t1);
SamplerState Sampler : register(s2);

float4 PSMain(FragmentIn Input) : SV_TARGET
{
    float4 Albedo = Texture.Sample(Sampler, Input.TextureCoords);
    return Albedo;
}