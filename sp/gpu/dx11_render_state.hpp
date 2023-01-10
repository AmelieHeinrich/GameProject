/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 16:07
 */

#pragma once

#include "dx11_common.hpp"

enum class render_state_fill_mode
{
    Fill = D3D11_FILL_SOLID,
    Line = D3D11_FILL_WIREFRAME
};

enum class render_state_cull_mode
{
    None = D3D11_CULL_NONE,
    Front = D3D11_CULL_FRONT,
    Back = D3D11_CULL_BACK
};

enum class render_state_op
{
    Never = D3D11_COMPARISON_NEVER,
    Less = D3D11_COMPARISON_LESS,
    Equal = D3D11_COMPARISON_EQUAL,
    LessEqual = D3D11_COMPARISON_LESS_EQUAL,
    Greater = D3D11_COMPARISON_GREATER,
    NotEqual = D3D11_COMPARISON_NOT_EQUAL,
    GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
    Always = D3D11_COMPARISON_ALWAYS
};

struct gpu_render_state
{
    render_state_fill_mode FillMode;
    render_state_cull_mode CullMode;
    render_state_op Depth;
    bool CounterClockwise;
    const char* Name;

    ID3D11RasterizerState *RS;
    ID3D11DepthStencilState *DS;
};

void GpuRenderStateCreate(gpu_render_state *State);
void GpuRenderStateFree(gpu_render_state *State);
void GpuRenderStateBind(gpu_render_state *State);
