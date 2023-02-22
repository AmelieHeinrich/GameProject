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

float4 PSMain(FragmentIn Input) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}