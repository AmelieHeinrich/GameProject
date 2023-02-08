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

struct SceneData
{
    row_major float4x4 View;
    row_major float4x4 Projection;
};

ConstantBuffer<SceneData> SceneBuffer : register(b0);

VertexOut VSMain(VertexIn Input)
{
    VertexOut Output = (VertexOut)0;
    Output.Position = mul(float4(Input.Position, 1.0f), SceneBuffer.View);
    Output.Position = mul(Output.Position, SceneBuffer.Projection);
    Output.Color = Input.Color;
    return Output;
}