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
    GpuSamplerInit(&Pass->Sampler, gpu_texture_address::Wrap, gpu_texture_filter::Nearest);

    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageInit(&Pass->RenderTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA16Float, gpu_image_usage::ImageUsageRenderTarget);
    GpuImageInit(&Pass->DepthTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::R32Depth, gpu_image_usage::ImageUsageDepthTarget);

    ShaderLibraryPush("Forward", "shaders/forward/Vertex.hlsl", "shaders/forward/Pixel.hlsl");
    Pass->Pipeline.Info.Formats.resize(1);
    Pass->Pipeline.Info.Shader = ShaderLibraryGet("Forward");
    Pass->Pipeline.Info.CullMode = cull_mode::None;
    Pass->Pipeline.Info.DepthFormat = gpu_image_format::R32Depth;
    Pass->Pipeline.Info.Formats[0] = gpu_image_format::RGBA16Float;
    Pass->Pipeline.Info.DepthFunc = depth_func::Less;
    Pass->Pipeline.Info.FillMode = fill_mode::Solid;
    Pass->Pipeline.Info.HasDepth = true;
    Pass->Pipeline.Info.Type = gpu_pipeline_type::Graphics;
    GpuPipelineCreateGraphics(&Pass->Pipeline);

    ModelLoad(&Pass->Model, "assets/models/DamagedHelmet.gltf");
    GpuBufferInit(&Pass->CameraBuffer, 256, 0, gpu_buffer_type::Uniform);
}

void ForwardPassExit(forward_pass *Pass)
{
    GpuSamplerFree(&Pass->Sampler);
    ModelFree(&Pass->Model);
    GpuBufferFree(&Pass->CameraBuffer);
    GpuPipelineFree(&Pass->Pipeline);
    GpuImageFree(&Pass->DepthTarget);
    GpuImageFree(&Pass->RenderTarget);
}

void ForwardPassUpdate(forward_pass *Pass, camera_data *Camera)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    gpu_command_buffer *Buffer = GpuGetImageCommandBuffer();

    GpuCommandBufferBegin(Buffer);
    GpuCommandBufferImageBarrier(Buffer, &Pass->RenderTarget, gpu_image_layout::ImageLayoutRenderTarget);
    GpuCommandBufferSetViewport(Buffer, Dimensions.Width, Dimensions.Height, 0, 0);
    GpuCommandBufferBindRenderTarget(Buffer, &Pass->RenderTarget, &Pass->DepthTarget);
    GpuCommandBufferClearColor(Buffer, &Pass->RenderTarget, 0.3f, 0.2f, 0.1f, 1.0f);
    GpuCommandBufferClearDepth(Buffer, &Pass->DepthTarget, 1.0f, 0.0f);
    GpuCommandBufferBindPipeline(Buffer, &Pass->Pipeline);
    GpuCommandBufferBindConstantBuffer(Buffer, gpu_pipeline_type::Graphics, &Pass->CameraBuffer, 0);
    GpuCommandBufferBindSampler(Buffer, gpu_pipeline_type::Graphics, &Pass->Sampler, 2);
    for (auto Mesh : Pass->Model.Meshes)
    {
        hmm_mat4 UploadMatrices[3] = { Camera->View, Camera->Projection, Mesh.Transform };
        GpuBufferUpload(&Pass->CameraBuffer, UploadMatrices, sizeof(UploadMatrices));

        GpuCommandBufferBindBuffer(Buffer, &Mesh.VertexBuffer);
        GpuCommandBufferBindBuffer(Buffer, &Mesh.IndexBuffer);
        GpuCommandBufferBindShaderResource(Buffer, gpu_pipeline_type::Graphics, &Mesh.Albedo, 1);
        GpuCommandBufferDrawIndexed(Buffer, Mesh.IndexCount);
    }
    GpuCommandBufferEnd(Buffer);
    GpuCommandBufferFlush(Buffer);
}

void ForwardPassResize(forward_pass *Pass, uint32_t Width, uint32_t Height)
{
    hmm_v2 Dimensions = GpuGetDimensions();

    GpuImageFree(&Pass->DepthTarget);
    GpuImageFree(&Pass->RenderTarget);
    GpuImageInit(&Pass->RenderTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::RGBA16Float, gpu_image_usage::ImageUsageRenderTarget);
    GpuImageInit(&Pass->DepthTarget, Dimensions.Width, Dimensions.Height, gpu_image_format::R32Depth, gpu_image_usage::ImageUsageDepthTarget);
}
