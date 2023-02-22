/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 20/02/2023 19:36
 */

#include "sharpness_pass.hpp"

#include "gpu/gpu_context.hpp"

#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"
#include "systems/event_system.hpp"

void SharpnessPassInit(sharpness_pass *Pass, gpu_image *Image)
{
    Pass->Image = Image;

    ShaderLibraryPush("Sharpness", "", "", "shaders/sharpness/Compute.hlsl");
    Pass->Pipeline.Info.Shader = ShaderLibraryGet("Sharpness");
    GpuPipelineCreateCompute(&Pass->Pipeline);
}

void SharpnessPassExit(sharpness_pass *Pass)
{
    GpuPipelineFree(&Pass->Pipeline);
}

void SharpnessPassUpdate(sharpness_pass *Pass, gpu_buffer *Settings)
{
    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    hmm_v2 Dimensions = GpuGetDimensions();

    GpuCommandBufferBegin(Buffer);

    GpuCommandBufferImageBarrier(Buffer, Pass->Image, gpu_image_layout::ImageLayoutStorage);

    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindStorageImage(Buffer, gpu_pipeline_type::Compute, Pass->Image, 0);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Compute, Settings, 1);
    GpuCommandBufferDispatch(Buffer, Dimensions.Width / 15, Dimensions.Height / 15, 1);

    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void SharpnessPassResize(sharpness_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *Image)
{
    Pass->Image = Image;
    hmm_v2 Dimensions = GpuGetDimensions();
}
