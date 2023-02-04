/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:37
 */

#pragma once

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
