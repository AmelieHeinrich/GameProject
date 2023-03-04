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
Texture2D NormalTexture : register(t2);
SamplerState Sampler : register(s3);

float4 PSMain(FragmentIn Input) : SV_TARGET
{
    float3 Normal = NormalTexture.Sample(Sampler, Input.TextureCoords).xyz;
    float4 Albedo = Texture.Sample(Sampler, Input.TextureCoords);
    Albedo.xyz *= normalize(Normal);
    return Albedo;
}