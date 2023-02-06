/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 16:37
 */

struct VertexIn
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

cbuffer SceneData : register(b0, space0)
{
    row_major float4x4 View;
    row_major float4x4 Projection;
};

VertexOut VSMain(VertexIn Input)
{
    VertexOut Output = (VertexOut)0;
    //Output.Position = float4(Input.Position, 1.0f);
    Output.Position = mul(float4(Input.Position, 1.0f), View);
    Output.Position = mul(Output.Position, Projection);
    Output.Color = Input.Color;
    return Output;
}