/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:38
 */

#include "pass_forward.hpp"

#include "game_data.hpp"
#include "gpu/dx11_context.hpp"

void ForwardPassInit(forward_pass *Pass)
{
    uint32_t Width = EgcI32(EgcFile, "width");
    uint32_t Height = EgcI32(EgcFile, "height");

    GpuTextureInit(&Pass->RenderTexture, DxRenderContext.Width, DxRenderContext.Height, DXGI_FORMAT_R8G8B8A8_UNORM, gpu_texture_bind::RenderTarget);
    GpuTextureInit(&Pass->DepthTexture, DxRenderContext.Width, DxRenderContext.Height, DXGI_FORMAT_D32_FLOAT, gpu_texture_bind::DepthStencil);
    GpuTextureInitRTV(&Pass->RenderTexture);
    GpuTextureInitDSV(&Pass->DepthTexture, DXGI_FORMAT_D32_FLOAT);
}

void ForwardPassFree(forward_pass *Pass)
{
    GpuTextureFree(&Pass->DepthTexture);
    GpuTextureFree(&Pass->RenderTexture);
}

void ForwardPassExecute(forward_pass *Pass)
{
    GpuResetRTV();

    GpuTextureBindRTV(&Pass->RenderTexture, &Pass->DepthTexture, HMM_Vec4(0.1f, 0.5f, 0.2f, 1.0f));

    GpuResetRTV();
}

void ForwardPassResize(forward_pass *Pass, uint32_t Width, uint32_t Height)
{
    GpuTextureFree(&Pass->DepthTexture);
    GpuTextureFree(&Pass->RenderTexture);

    GpuTextureInit(&Pass->RenderTexture, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, gpu_texture_bind::RenderTarget);
    GpuTextureInit(&Pass->DepthTexture, Width, Height, DXGI_FORMAT_D32_FLOAT, gpu_texture_bind::DepthStencil);
    GpuTextureInitRTV(&Pass->RenderTexture);
    GpuTextureInitDSV(&Pass->DepthTexture, DXGI_FORMAT_D32_FLOAT);
}
