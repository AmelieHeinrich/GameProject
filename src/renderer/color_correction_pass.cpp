/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 20/02/2023 18:52
 */

#include "color_correction_pass.hpp"

#include "gpu/gpu_context.hpp"

#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"
#include "systems/event_system.hpp"

void ColorCorrectionPassInit(color_correction_pass *Pass, gpu_image *HDRImage)
{
    Pass->HDRImage = HDRImage;
    
    hmm_v2 Dimensions = GpuGetDimensions();

    ShaderLibraryPush("Color Correction", "", "", "shaders/color_correction/Compute.hlsl");
    Pass->Pipeline.Info.Shader = ShaderLibraryGet("Color Correction");
    GpuPipelineCreateCompute(&Pass->Pipeline);
}

void ColorCorrectionPassExit(color_correction_pass *Pass)
{
    GpuPipelineFree(&Pass->Pipeline);
}

void ColorCorrectionPassUpdate(color_correction_pass *Pass, gpu_buffer *Settings)
{
    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    hmm_v2 Dimensions = GpuGetDimensions();

    GpuCommandBufferBegin(Buffer);

    GpuCommandBufferImageBarrier(Buffer, Pass->HDRImage, gpu_image_layout::ImageLayoutStorage);

    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindStorageImage(Buffer, gpu_pipeline_type::Compute, Pass->HDRImage, 0);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Compute, Settings, 1);
    GpuCommandBufferDispatch(Buffer, Dimensions.Width / 16, Dimensions.Height / 16, 1);

    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void ColorCorrectionPassResize(color_correction_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *HDRImage)
{
    Pass->HDRImage = HDRImage;
    hmm_v2 Dimensions = GpuGetDimensions();
}
