/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:37
 */

#pragma once

#include "gpu_buffer.hpp"

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
// TODO(amelie.h): Bind texture
void GpuCommandBufferBindBuffer(gpu_command_buffer *Command, gpu_buffer *Buffer);
// TODO(amelie.h): Bind descriptor
// TODO(amelie.h): Bind pipeline
// TODO(amelie.h): Clear
// TODO(amelie.h): Set viewport
// TODO(amelie.h): Draw
// TODO(amelie.h): Draw indexed
// TODO(amelie.h): Dispatch
// TODO(amelie.h): Barrier
// TODO(amelie.h): Blit
// TODO(amelie.h): Flush
