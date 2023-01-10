/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/01/2023 22:11
 */

#include "dx11_common.hpp"

#include <cstdint>

enum class gpu_buffer_usage
{
    Vertex,
    Index,
    Constant,
    UnorderedAccess
};

enum class gpu_resource_bind
{
    Vertex,
    Pixel,
    Compute
};

struct gpu_buffer
{
    ID3D11Buffer* Buffer;
    int64_t Stride;
};

void GpuBufferCreate(gpu_buffer *Buffer, int64_t Size, int64_t Stride, gpu_buffer_usage Usage);
void GpuBufferFree(gpu_buffer *Buffer);
void GpuBufferUploadData(gpu_buffer *Buffer, const void *Data);
void GpuBufferBindVertex(gpu_buffer *Buffer);
void GpuBufferBindIndex(gpu_buffer *Buffer);
void GpuBufferBindConstant(gpu_buffer *Buffer, int Binding, gpu_resource_bind Bind);
// TODO(amelie.h): Unordered access view
