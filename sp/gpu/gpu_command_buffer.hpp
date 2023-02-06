/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:37
 */

#pragma once

#include "gpu_buffer.hpp"
#include "gpu_image.hpp"

enum class gpu_command_buffer_type
{
    Graphics,
    Compute,
    Upload
};

struct gpu_command_buffer
{
    void *Private;
    gpu_command_buffer_type Type;
};

void GpuCommandBufferInit(gpu_command_buffer *Buffer, gpu_command_buffer_type Type);
void GpuCommandBufferFree(gpu_command_buffer *Buffer);
void GpuCommandBufferBindBuffer(gpu_command_buffer *Command, gpu_buffer *Buffer);
// TODO(amelie.h): Bind pipeline
// TODO(amelie.h): Bind descriptor
void GpuCommandBufferClearColor(gpu_command_buffer *Command, gpu_image *Image, float Red, float Green, float Blue, float Alpha);
// TODO(amelie.h): Clear depth
void GpuCommandBufferSetViewport(gpu_command_buffer *Command, float Width, float Height, float X, float Y);
void GpuCommandBufferDraw(gpu_command_buffer *Command, int VertexCount);
void GpuCommandBufferDrawIndexed(gpu_command_buffer *Command, int IndexCount);
void GpuCommandBufferDispatch(gpu_command_buffer *Command, int X, int Y, int Z);
// TODO(amelie.h): Barrier buffer
void GpuCommandBufferImageBarrier(gpu_command_buffer *Command, gpu_image *Image, gpu_image_layout New);
void GpuCommandBufferBlit(gpu_command_buffer *Command, gpu_image *Source, gpu_image *Dest);
void GpuCommandBufferBegin(gpu_command_buffer *Command);
void GpuCommandBufferEnd(gpu_command_buffer *Command);
void GpuCommandBufferFlush(gpu_command_buffer *Command);
