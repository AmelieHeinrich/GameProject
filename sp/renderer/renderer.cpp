/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:46
 */

#include "renderer.hpp"

#include "gpu/dx11/dx11_context.hpp"

renderer_data Renderer;

void RendererInit()
{
    ForwardPassInit(&Renderer.Forward);
}

void RendererExit()
{
    ForwardPassFree(&Renderer.Forward);
}

void RendererUpdate()
{
    ForwardPassExecute(&Renderer.Forward);

    DxRenderContext.DeviceContext->CopyResource(DxRenderContext.Buffers[0].Buffer, Renderer.Forward.RenderTexture.Texture);
}

void RendererResize(uint32_t Width, uint32_t Height)
{
    ForwardPassResize(&Renderer.Forward, Width, Height);
}
