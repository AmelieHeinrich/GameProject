/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 16:39
 */

struct FragmentIn
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

float4 PSMain(FragmentIn Input) : SV_TARGET
{
    return float4(Input.Color, 1.0);
}