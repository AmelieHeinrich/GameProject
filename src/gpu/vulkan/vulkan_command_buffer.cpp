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

}

void GpuCommandBufferDraw(gpu_command_buffer *Command, int VertexCount)
{

}

void GpuCommandBufferDrawIndexed(gpu_command_buffer *Command, int IndexCount)
{

}

void GpuCommandBufferDispatch(gpu_command_buffer *Command, int X, int Y, int Z)
{

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
