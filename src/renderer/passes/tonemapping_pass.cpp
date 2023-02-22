/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 17:57
 */

#include "tonemapping_pass.hpp"

#include "gpu/gpu_context.hpp"

#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"
#include "systems/event_system.hpp"

void TonemappingPassInit(tonemapping_pass *Pass, gpu_image *HDRImage)
{
    Pass->HDRImage = HDRImage;
    
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageInit(&Pass->LDRImage, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA8, gpu_image_usage::ImageUsageRenderTarget);

    ShaderLibraryPush("Tonemapping", "", "", "shaders/tonemapping/Compute.hlsl");
    Pass->Pipeline.Info.Shader = ShaderLibraryGet("Tonemapping");
    GpuPipelineCreateCompute(&Pass->Pipeline);
}

void TonemappingPassExit(tonemapping_pass *Pass)
{
    GpuPipelineFree(&Pass->Pipeline);
    GpuImageFree(&Pass->LDRImage);
}

void TonemappingPassUpdate(tonemapping_pass *Pass, gpu_buffer *Settings)
{
    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    hmm_v2 Dimensions = GpuGetDimensions();

    GpuCommandBufferBegin(Buffer);

    GpuCommandBufferImageBarrier(Buffer, Pass->HDRImage, gpu_image_layout::ImageLayoutShaderResource);
    GpuCommandBufferImageBarrier(Buffer, &Pass->LDRImage, gpu_image_layout::ImageLayoutStorage);

    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindShaderResource(Buffer, gpu_pipeline_type::Compute, Pass->HDRImage, 0);
    GpuCommandBufferBindStorageImage(Buffer, gpu_pipeline_type::Compute, &Pass->LDRImage, 1);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Compute, Settings, 2);
    GpuCommandBufferDispatch(Buffer, Dimensions.Width / 16, Dimensions.Height / 16, 1);

    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void TonemappingPassResize(tonemapping_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *HDRImage)
{
    Pass->HDRImage = HDRImage;
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageFree(&Pass->LDRImage);
    GpuImageInit(&Pass->LDRImage, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA8, gpu_image_usage::ImageUsageRenderTarget);
}
