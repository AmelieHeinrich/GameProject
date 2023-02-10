/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:46
 */

#include "renderer.hpp"

#include "gpu/gpu_context.hpp"
#include "systems/event_system.hpp"

#include <stdlib.h>

struct renderer_data
{
    forward_pass Forward;
};

renderer_data Renderer;

bool RendererShaderRecompile(event_type Type, void *Sender, void *Listener, event_data Data)
{
    GpuWait();
    ForwardPassExit(&Renderer.Forward);
    ForwardPassInit(&Renderer.Forward);
    return false;
}

void RendererInit()
{
    EventSystemRegister(event_type::ShaderRecompile, nullptr, RendererShaderRecompile);
    ForwardPassInit(&Renderer.Forward);
}

void RendererExit()
{
    ForwardPassExit(&Renderer.Forward);
}

void RendererStartSync()
{
    GpuBeginFrame();
}

void RendererConstructFrame(camera_data *Camera)
{
    ForwardPassUpdate(&Renderer.Forward, Camera);

    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();
    gpu_image *Image = GpuGetSwapChainImage();

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutCopyDest);
    GpuCommandBufferImageBarrier(Buffer, &Renderer.Forward.RenderTarget, gpu_image_layout::ImageLayoutCopySource);
    GpuCommandBufferBlit(Buffer, &Renderer.Forward.RenderTarget, Image);
    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutCommon);
    GpuCommandBufferImageBarrier(Buffer, &Renderer.Forward.RenderTarget, gpu_image_layout::ImageLayoutRenderTarget);
    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void RendererStartRender()
{
    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();
    gpu_image *Image = GpuGetSwapChainImage();

    hmm_v2 Dimensions = GpuGetDimensions();

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutRenderTarget);
    GpuCommandBufferSetViewport(Buffer, Dimensions.Width, Dimensions.Height, 0, 0);
    GpuCommandBufferBindRenderTarget(Buffer, Image, nullptr);
}

void RendererEndRender()
{
    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();
    gpu_image *Image = GpuGetSwapChainImage();

    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutPresent);
    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void RendererEndSync()
{
    GpuEndFrame();
}

void RendererResize(uint32_t Width, uint32_t Height)
{
    GpuResize(Width, Height);
    ForwardPassResize(&Renderer.Forward, Width, Height);
}