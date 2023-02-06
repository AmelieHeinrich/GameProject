/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:46
 */

#include "renderer.hpp"

#include "gpu/gpu_context.hpp"

#include <stdlib.h>

void RendererInit()
{
    return;
}

void RendererExit()
{
    return;
}

void RendererStartSync()
{
    GpuBeginFrame();
}

void RendererConstructFrame()
{
    // Execute render graph, and copy to swapchain
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
    GpuCommandBufferClearColor(Buffer, Image, 0.2f, 0.3f, 0.4f, 1.0f);
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
}
