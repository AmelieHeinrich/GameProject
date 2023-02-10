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
#include "systems/event_system.hpp"

void ForwardPassInit(forward_pass *Pass)
{
    cpu_image CpuImage;
    CpuImageLoad(&CpuImage, "assets/gfx/texture.jpg");
    GpuImageInitFromCPU(&Pass->Texture, &CpuImage);
    CpuImageFree(&CpuImage);

    GpuSamplerInit(&Pass->Sampler, gpu_texture_address::Wrap, gpu_texture_filter::Nearest);

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
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t Indices[] = {
        0, 1, 3,
        1, 2, 3  
    };

    GpuBufferInit(&Pass->VertexBuffer, sizeof(Vertices), sizeof(float) * 8, gpu_buffer_type::Vertex);
    GpuBufferUpload(&Pass->VertexBuffer, Vertices, sizeof(Vertices));
    GpuBufferInit(&Pass->IndexBuffer, sizeof(Indices), sizeof(uint32_t), gpu_buffer_type::Index);
    GpuBufferUpload(&Pass->IndexBuffer, Indices, sizeof(Indices));

    GpuBufferInit(&Pass->CameraBuffer, 256, 0, gpu_buffer_type::Uniform);
}

void ForwardPassExit(forward_pass *Pass)
{
    GpuSamplerFree(&Pass->Sampler);
    GpuImageFree(&Pass->Texture);
    GpuBufferFree(&Pass->IndexBuffer);
    GpuBufferFree(&Pass->CameraBuffer);
    GpuBufferFree(&Pass->VertexBuffer);
    GpuPipelineFree(&Pass->Pipeline);
    GpuImageFree(&Pass->DepthTarget);
    GpuImageFree(&Pass->RenderTarget);
}

void ForwardPassUpdate(forward_pass *Pass, camera_data *Camera)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    hmm_mat4 UploadMatrices[2] = { Camera->View, Camera->Projection };
    GpuBufferUpload(&Pass->CameraBuffer, UploadMatrices, sizeof(UploadMatrices));

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferSetViewport(Buffer, Dimensions.Width, Dimensions.Height, 0, 0);
    GpuCommandBufferBindRenderTarget(Buffer, &Pass->RenderTarget, &Pass->DepthTarget);
    GpuCommandBufferClearColor(Buffer, &Pass->RenderTarget, 0.3f, 0.2f, 0.1f, 1.0f);
    GpuCommandBufferClearDepth(Buffer, &Pass->DepthTarget, 1.0f, 0.0f);
    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindBuffer(Buffer, &Pass->VertexBuffer);
    GpuCommandBufferBindBuffer(Buffer, &Pass->IndexBuffer);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Graphics, &Pass->CameraBuffer, 0);
    GpuCommandBufferBindShaderResource(Buffer, gpu_pipeline_type::Graphics, &Pass->Texture, 1);
    GpuCommandBufferBindSampler(Buffer, gpu_pipeline_type::Graphics, &Pass->Sampler, 2);
    GpuCommandBufferDrawIndexed(Buffer, 6);
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
