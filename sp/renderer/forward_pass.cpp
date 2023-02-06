/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 16:32
 */

#include "forward_pass.hpp"

#include "gpu/gpu_context.hpp"

#include "systems/log_system.hpp"

void ForwardPassInit(forward_pass *Pass)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageInit(&Pass->RenderTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA8, gpu_image_usage::ImageUsageRenderTarget);
    GpuImageInit(&Pass->DepthTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::R32Depth, gpu_image_usage::ImageUsageDepthTarget);
}

void ForwardPassExit(forward_pass *Pass)
{
    GpuImageFree(&Pass->DepthTarget);
    GpuImageFree(&Pass->RenderTarget);
}

void ForwardPassUpdate(forward_pass *Pass)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferSetViewport(Buffer, Dimensions.Width, Dimensions.Height, 0, 0);
    GpuCommandBufferBindRenderTarget(Buffer, &Pass->RenderTarget, &Pass->DepthTarget);
    GpuCommandBufferClearColor(Buffer, &Pass->RenderTarget, 0.3f, 0.2f, 0.1f, 1.0f);
    GpuCommandBufferClearDepth(Buffer, &Pass->DepthTarget, 1.0f, 0.0f);
    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void ForwardPassResize(forward_pass *Pass, uint32_t Width, uint32_t Height)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageFree(&Pass->DepthTarget);
    GpuImageFree(&Pass->RenderTarget);
    GpuImageInit(&Pass->RenderTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA8, gpu_image_usage::ImageUsageRenderTarget);
    GpuImageInit(&Pass->DepthTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::R32Depth, gpu_image_usage::ImageUsageDepthTarget);
}
