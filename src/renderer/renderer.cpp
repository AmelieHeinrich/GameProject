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
    color_correction_pass ColorCorrection;
    sharpness_pass SharpnessPass;
    tonemapping_pass Tonemapping;

    renderer_settings Settings;
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

    TonemappingPassExit(&Renderer.Tonemapping);
    SharpnessPassExit(&Renderer.SharpnessPass);
    ColorCorrectionPassExit(&Renderer.ColorCorrection);
    ForwardPassExit(&Renderer.Forward);

    ForwardPassInit(&Renderer.Forward);
    ColorCorrectionPassInit(&Renderer.ColorCorrection, &Renderer.Forward.RenderTarget);
    SharpnessPassInit(&Renderer.SharpnessPass, &Renderer.Forward.RenderTarget);
    TonemappingPassInit(&Renderer.Tonemapping, &Renderer.Forward.RenderTarget);

    return false;
}

void RendererInit()
{
    EventSystemRegister(event_type::ShaderRecompile, nullptr, RendererShaderRecompile);
    EventSystemRegister(event_type::KeyPressed, nullptr, RendererOnKeyPressed);
 
    Renderer.Settings.Wireframe = false;
    Renderer.Settings.EnableSharpness = false;
    Renderer.Settings.EnableColorCorrection = true;

    Renderer.Settings.Settings.Tonemapper = tonemapping_algorithm::Filmic;
    Renderer.Settings.Settings.Exposure = 2.2f;
    Renderer.Settings.Settings.Temperature = 0.0f;
    Renderer.Settings.Settings.Tint = 0.0f;
    Renderer.Settings.Settings.Contrast = HMM_Vec3(1.0f, 1.0f, 1.0f);
    Renderer.Settings.Settings.LinearMidPoint = HMM_Vec3(0.0f, 0.0f, 0.0f);
    Renderer.Settings.Settings.Brightness = HMM_Vec3(0.0f, 0.0f, 0.0f);
    Renderer.Settings.Settings.ColorFilter = HMM_Vec3(0.0f, 0.0f, 0.0f);
    Renderer.Settings.Settings.ColorFilterIntensity = 1.0f;
    Renderer.Settings.Settings.Saturation = HMM_Vec3(1.0f, 1.0f, 1.0f);
    Renderer.Settings.Settings.SharpnessStrength = 1.0f;

    RendererSettingsInit(&Renderer.Settings);
    ForwardPassInit(&Renderer.Forward);
    ColorCorrectionPassInit(&Renderer.ColorCorrection, &Renderer.Forward.RenderTarget);
    SharpnessPassInit(&Renderer.SharpnessPass, &Renderer.Forward.RenderTarget);
    TonemappingPassInit(&Renderer.Tonemapping, &Renderer.Forward.RenderTarget);
}

void RendererExit()
{
    ForwardPassExit(&Renderer.Forward);
    SharpnessPassExit(&Renderer.SharpnessPass);
    ColorCorrectionPassExit(&Renderer.ColorCorrection);
    TonemappingPassExit(&Renderer.Tonemapping);
    RendererSettingsFree(&Renderer.Settings);
}

void RendererStartSync()
{
    GpuBeginFrame();
}

void RendererConstructFrame(camera_data *Camera)
{
    RendererSettingsUpdate(&Renderer.Settings);
    ForwardPassUpdate(&Renderer.Forward, Camera, Renderer.Settings.Wireframe);
    if (Renderer.Settings.EnableColorCorrection)
        ColorCorrectionPassUpdate(&Renderer.ColorCorrection, &Renderer.Settings.Buffer);
    if (Renderer.Settings.EnableSharpness)
        SharpnessPassUpdate(&Renderer.SharpnessPass, &Renderer.Settings.Buffer);
    TonemappingPassUpdate(&Renderer.Tonemapping, &Renderer.Settings.Buffer);

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
    ColorCorrectionPassResize(&Renderer.ColorCorrection, Width, Height, &Renderer.Forward.RenderTarget);
    SharpnessPassResize(&Renderer.SharpnessPass, Width, Height, &Renderer.Forward.RenderTarget);
    TonemappingPassResize(&Renderer.Tonemapping, Width, Height, &Renderer.Forward.RenderTarget);
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

renderer_settings *RendererGetSettings()
{
    return &Renderer.Settings;
}
