/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:46
 */

#include "renderer.hpp"

#include "gpu/gpu_context.hpp"
#include "systems/event_system.hpp"
#include "systems/input_types.hpp"

#include <stdlib.h>

struct renderer_data
{
    forward_pass Forward;
    tonemapping_pass Tonemapping;
};

renderer_data Renderer;

bool RendererOnKeyPressed(event_type Type, void *Sender, void *Listener, event_data Data)
{   
    if (Data.data.u32[0] == (uint32_t)keyboard_key::F2)
        RendererScreenshot();
    return false;
}

bool RendererShaderRecompile(event_type Type, void *Sender, void *Listener, event_data Data)
{
    GpuWait();
    ForwardPassExit(&Renderer.Forward);
    TonemappingPassExit(&Renderer.Tonemapping);
    ForwardPassInit(&Renderer.Forward);
    TonemappingPassInit(&Renderer.Tonemapping, &Renderer.Forward.RenderTarget);
    return false;
}

void RendererInit()
{
    EventSystemRegister(event_type::ShaderRecompile, nullptr, RendererShaderRecompile);
    EventSystemRegister(event_type::KeyPressed, nullptr, RendererOnKeyPressed);
    ForwardPassInit(&Renderer.Forward);
    TonemappingPassInit(&Renderer.Tonemapping, &Renderer.Forward.RenderTarget);
}

void RendererExit()
{
    ForwardPassExit(&Renderer.Forward);
    TonemappingPassExit(&Renderer.Tonemapping);
}

void RendererStartSync()
{
    GpuBeginFrame();
}

void RendererConstructFrame(camera_data *Camera)
{
    ForwardPassUpdate(&Renderer.Forward, Camera);
    TonemappingPassUpdate(&Renderer.Tonemapping);

    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();
    gpu_image *Image = GpuGetSwapChainImage();

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutCopyDest);
    GpuCommandBufferImageBarrier(Buffer, &Renderer.Tonemapping.LDRImage, gpu_image_layout::ImageLayoutCopySource);
    GpuCommandBufferBlit(Buffer, &Renderer.Tonemapping.LDRImage, Image);
    GpuCommandBufferImageBarrier(Buffer, Image, gpu_image_layout::ImageLayoutCommon);
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

void RendererScreenshot()
{
    GpuWait();

    gpu_image *Image = GpuGetSwapChainImage();

    gpu_buffer Temporary;
    GpuBufferInitForCopy(&Temporary, Image->Width * Image->Height * 4);

    gpu_command_buffer Buffer;
    GpuCommandBufferInit(&Buffer, gpu_command_buffer_type::Graphics);
    GpuCommandBufferScreenshot(&Buffer, Image, &Temporary);
    GpuCommandBufferFree(&Buffer);

    GpuBufferFree(&Temporary);
}
