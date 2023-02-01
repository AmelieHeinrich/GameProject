/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 16:15
 */

#include "dx11_render_state.hpp"
#include "dx11_context.hpp"

#include "systems/log_system.hpp"

void GpuRenderStateCreate(gpu_render_state *State)
{
    D3D11_RASTERIZER_DESC Desc = {};
    Desc.CullMode = (D3D11_CULL_MODE)State->CullMode;
    Desc.FillMode = (D3D11_FILL_MODE)State->FillMode;
    Desc.FrontCounterClockwise = State->CounterClockwise;

    HRESULT Result = DxRenderContext.Device->CreateRasterizerState(&Desc, &State->RS);
    if (FAILED(Result))
        LogError("Failed to create rasterizer state!");

    D3D11_DEPTH_STENCIL_DESC DepthDesc = {};
    DepthDesc.DepthEnable = true;
    DepthDesc.DepthFunc = (D3D11_COMPARISON_FUNC)State->Depth;
    DepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    Result = DxRenderContext.Device->CreateDepthStencilState(&DepthDesc, &State->DS);
    if (FAILED(Result))
        LogError("Failed to create depth stencil state!");
}

void GpuRenderStateFree(gpu_render_state *State)
{
    SafeRelease(State->DS);
    SafeRelease(State->RS);
}

void GpuRenderStateBind(gpu_render_state *State)
{
    DxRenderContext.DeviceContext->RSSetState(State->RS);
    DxRenderContext.DeviceContext->OMSetDepthStencilState(State->DS, 0);
}

