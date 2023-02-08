/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 16:32
 */

#include "forward_pass.hpp"

#include "gpu/gpu_context.hpp"

#include "systems/shader_system.hpp"
#include "systems/log_system.hpp"

void ForwardPassInit(forward_pass *Pass)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageInit(&Pass->RenderTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA8, gpu_image_usage::ImageUsageRenderTarget);
    GpuImageInit(&Pass->DepthTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::R32Depth, gpu_image_usage::ImageUsageDepthTarget);

    ShaderLibraryPush("Forward", "shaders/forward/Vertex.hlsl", "shaders/forward/Pixel.hlsl");
    Pass->Pipeline.Info.Formats.resize(1);
    Pass->Pipeline.Info.Shader = ShaderLibraryGet("Forward");
    Pass->Pipeline.Info.CullMode = cull_mode::None;
    Pass->Pipeline.Info.DepthFormat = gpu_image_format::R32Depth;
    Pass->Pipeline.Info.Formats[0] = gpu_image_format::RGBA8;
    Pass->Pipeline.Info.DepthFunc = depth_func::Less;
    Pass->Pipeline.Info.FillMode = fill_mode::Solid;
    Pass->Pipeline.Info.HasDepth = true;
    Pass->Pipeline.Info.Type = gpu_pipeline_type::Graphics;
    GpuPipelineCreateGraphics(&Pass->Pipeline);
    
    float Vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GpuBufferInit(&Pass->TriangleBuffer, sizeof(Vertices), sizeof(float) * 6, gpu_buffer_type::Vertex);
    GpuBufferUpload(&Pass->TriangleBuffer, Vertices, sizeof(Vertices));

    GpuBufferInit(&Pass->CameraBuffer, 256, 0, gpu_buffer_type::Uniform);
}

void ForwardPassExit(forward_pass *Pass)
{
    GpuBufferFree(&Pass->CameraBuffer);
    GpuBufferFree(&Pass->TriangleBuffer);
    GpuPipelineFree(&Pass->Pipeline);
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
    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindBuffer(Buffer, &Pass->TriangleBuffer);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Graphics, &Pass->CameraBuffer, GpuPipelineGetDescriptor(&Pass->Pipeline, "SceneBuffer"));
    GpuCommandBufferDraw(Buffer, 3);
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
