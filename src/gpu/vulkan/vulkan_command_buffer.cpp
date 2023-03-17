/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:58
 */

#include "vulkan_command_buffer.hpp"

void GpuCommandBufferInit(gpu_command_buffer *Buffer, gpu_command_buffer_type Type)
{

}

void GpuCommandBufferFree(gpu_command_buffer *Buffer)
{

}

void GpuCommandBufferBindBuffer(gpu_command_buffer *Command, gpu_buffer *Buffer)
{

}

void GpuCommandBufferBindPipeline(gpu_command_buffer *Command, gpu_pipeline *Pipeline)
{

}

void GpuCommandBufferBindConstantBuffer(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_buffer *Buffer, int Offset)
{

}

void GpuCommandBufferBindShaderResource(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_image *Image, int Offset)
{

}

void GpuCommandBufferBindStorageImage(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_image *Image, int Offset)
{

}

void GpuCommandBufferBindStorageBuffer(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_buffer *Buffer, int Offset)
{

}

void GpuCommandBufferBindSampler(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_sampler *Sampler, int Offset)
{

}

void GpuCommandBufferBindRenderTarget(gpu_command_buffer *Command, gpu_image *Image, gpu_image *Depth)
{

}

void GpuCommandBufferClearColor(gpu_command_buffer *Command, gpu_image *Image, float Red, float Green, float Blue, float Alpha)
{

}

void GpuCommandBufferClearDepth(gpu_command_buffer *Command, gpu_image *Image, float Depth, float Stencil)
{

}

void GpuCommandBufferSetViewport(gpu_command_buffer *Command, float Width, float Height, float X, float Y)
{
    vulkan_command_buffer *Buffer = (vulkan_command_buffer*)Command->Private;

    VkViewport Viewport = {};
    Viewport.width = Width;
    Viewport.height = Height;
    Viewport.minDepth = 0.0f;
    Viewport.maxDepth = 1.0f;
    Viewport.x = X;
    Viewport.y = Y;

    VkRect2D Scissor = {};
    Scissor.extent.width = Width;
    Scissor.extent.height = Height;
    Scissor.offset.x = X;
    Scissor.offset.y = Y;

    vkCmdSetViewport(Buffer->CommandBuffer, 0, 1, &Viewport);
    vkCmdSetScissor(Buffer->CommandBuffer, 0, 1, &Scissor);
}

void GpuCommandBufferDraw(gpu_command_buffer *Command, int VertexCount)
{
    vulkan_command_buffer *Buffer = (vulkan_command_buffer*)Command->Private;
    
    vkCmdDraw(Buffer->CommandBuffer, VertexCount, 1, 0, 0);
}

void GpuCommandBufferDrawIndexed(gpu_command_buffer *Command, int IndexCount)
{
    vulkan_command_buffer *Buffer = (vulkan_command_buffer*)Command->Private;

    vkCmdDrawIndexed(Buffer->CommandBuffer, IndexCount, 1, 0, 0, 0);
}

void GpuCommandBufferDispatch(gpu_command_buffer *Command, int X, int Y, int Z)
{
    vulkan_command_buffer *Buffer = (vulkan_command_buffer*)Command->Private;

    vkCmdDispatch(Buffer->CommandBuffer, X, Y, Z);
}

void GpuCommandBufferBeginPipelineStatistics(gpu_command_buffer *Command, gpu_pipeline_profiler *Profiler)
{

}

void GpuCommandBufferEndPipelineStatistics(gpu_command_buffer *Command, gpu_pipeline_profiler *Profiler)
{

}

void GpuCommandBufferBufferBarrier(gpu_command_buffer *Command, gpu_buffer *Buffer, gpu_buffer_layout Old, gpu_buffer_layout New)
{

}

void GpuCommandBufferImageBarrier(gpu_command_buffer *Command, gpu_image *Image, gpu_image_layout New)
{

}

void GpuCommandBufferBlit(gpu_command_buffer *Command, gpu_image *Source, gpu_image *Dest)
{

}

void GpuCommandBufferCopyBufferToTexture(gpu_command_buffer *Command, gpu_buffer *Source, gpu_image *Dest)
{

}

void GpuCommandBufferCopyTextureToBuffer(gpu_command_buffer *Command, gpu_image *Source, gpu_buffer *Dest)
{

}

void GpuCommandBufferCopyBufferToBuffer(gpu_command_buffer *Command, gpu_buffer *Source, gpu_buffer *Dest)
{

}

void GpuCommandBufferBegin(gpu_command_buffer *Command)
{

}

void GpuCommandBufferEnd(gpu_command_buffer *Command)
{

}

void GpuCommandBufferFlush(gpu_command_buffer *Command)
{

}

void GpuCommandBufferScreenshot(gpu_command_buffer *Command, gpu_image *Image, gpu_buffer *Temporary)
{
    
}
