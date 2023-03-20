/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:58
 */

#include "vulkan_buffer.hpp"

#include "vulkan_context.hpp"

#include <systems/log_system.hpp>

VkBufferUsageFlagBits BufferUsageToVulkan(gpu_buffer_type Type)
{
    switch (Type)
    {
        case gpu_buffer_type::Vertex:
            return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case gpu_buffer_type::Index:
            return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case gpu_buffer_type::Uniform:
            return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        case gpu_buffer_type::Storage:
            return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }
}

VmaMemoryUsage BufferUsageToMemoryUsage(gpu_buffer_type Type)
{
    switch (Type)
    {
        case gpu_buffer_type::Vertex:
            return VMA_MEMORY_USAGE_GPU_ONLY;
        case gpu_buffer_type::Index:
            return VMA_MEMORY_USAGE_GPU_ONLY;
        case gpu_buffer_type::Uniform:
            return VMA_MEMORY_USAGE_CPU_ONLY;
        case gpu_buffer_type::Storage:
            return VMA_MEMORY_USAGE_CPU_TO_GPU;
    }
}

void GpuBufferInit(gpu_buffer *Buffer, uint64_t Size, uint64_t Stride, gpu_buffer_type Type)
{
    Buffer->Size = Size;
    Buffer->Stride = Stride;
    Buffer->Type = Type;
    Buffer->Reserved = new vulkan_buffer;

    vulkan_buffer *Private = (vulkan_buffer*)Buffer->Reserved;

    VkBufferCreateInfo BufferCreateInfo = {};
    BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferCreateInfo.size = Size;
    BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    BufferCreateInfo.usage = BufferUsageToVulkan(Type);

    VmaAllocationCreateInfo AllocationInfo = {};
    AllocationInfo.usage = BufferUsageToMemoryUsage(Type);
    AllocationInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VkResult Result = vmaCreateBuffer(VK.Allocator, &BufferCreateInfo, &AllocationInfo, &Private->Buffer, &Private->Allocation, nullptr);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create vulkan buffer!");
}

void GpuBufferInitForUpload(gpu_buffer *Buffer, uint64_t Size)
{
    Buffer->Size = Size;
    Buffer->Reserved = new vulkan_buffer;

    vulkan_buffer *Private = (vulkan_buffer*)Buffer->Reserved;

    VkBufferCreateInfo BufferCreateInfo = {};
    BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferCreateInfo.size = Size;
    BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    BufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo AllocationInfo = {};
    AllocationInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    AllocationInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

    VkResult Result = vmaCreateBuffer(VK.Allocator, &BufferCreateInfo, &AllocationInfo, &Private->Buffer, &Private->Allocation, nullptr);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create vulkan buffer!");
}

void GpuBufferInitForCopy(gpu_buffer *Buffer, uint64_t Size)
{
    Buffer->Size = Size;
    Buffer->Reserved = new vulkan_buffer;

    vulkan_buffer *Private = (vulkan_buffer*)Buffer->Reserved;

    VkBufferCreateInfo BufferCreateInfo = {};
    BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    BufferCreateInfo.size = Size;
    BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    BufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    VmaAllocationCreateInfo AllocationInfo = {};
    AllocationInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    VkResult Result = vmaCreateBuffer(VK.Allocator, &BufferCreateInfo, &AllocationInfo, &Private->Buffer, &Private->Allocation, nullptr);
    if (Result != VK_SUCCESS)
        LogError("VULKAN: Failed to create vulkan buffer!");
}

void GpuBufferFree(gpu_buffer *Buffer)
{
    vulkan_buffer *Private = (vulkan_buffer*)Buffer->Reserved;

    vmaDestroyBuffer(VK.Allocator, Private->Buffer, Private->Allocation);
}

void GpuBufferUpload(gpu_buffer *Buffer, const void *Data, uint64_t Size)
{
    vulkan_buffer *Private = (vulkan_buffer*)Buffer->Reserved;
    
    if (Buffer->Type == gpu_buffer_type::Uniform)
    {
        void *Pointer;
        VkResult Result = vmaMapMemory(VK.Allocator, Private->Allocation, &Pointer);
        if (Result != VK_SUCCESS)
            LogError("VULKAN: Failed to map buffer in range (0, %d)", Size);
        memcpy(Pointer, Data, Size);
        vmaUnmapMemory(VK.Allocator, Private->Allocation);
    }
    else
    {
        gpu_buffer Temp;
        GpuBufferInitForUpload(&Temp, Size);

        vulkan_buffer *TempPrivate = (vulkan_buffer*)Temp.Reserved;
        void *Pointer;
        VkResult Result = vmaMapMemory(VK.Allocator, TempPrivate->Allocation, &Pointer);
        if (Result != VK_SUCCESS)
            LogError("VULKAN: Failed to map buffer in range (0, %d)", Size);
        memcpy(Pointer, Data, Size);
        vmaUnmapMemory(VK.Allocator, TempPrivate->Allocation);
    
        gpu_command_buffer Command;
        GpuCommandBufferInit(&Command, gpu_command_buffer_type::Graphics);
        GpuCommandBufferBegin(&Command);
        GpuCommandBufferBufferBarrier(&Command, &Temp, gpu_buffer_layout::ImageLayoutCommon, gpu_buffer_layout::ImageLayoutCopySource);
        GpuCommandBufferBufferBarrier(&Command, Buffer, gpu_buffer_layout::ImageLayoutCommon, gpu_buffer_layout::ImageLayoutCopyDest);
        GpuCommandBufferCopyBufferToBuffer(&Command, &Temp, Buffer);
        GpuCommandBufferBufferBarrier(&Command, Buffer, gpu_buffer_layout::ImageLayoutCopyDest, gpu_buffer_layout::ImageLayoutGenericRead);
        GpuCommandBufferEnd(&Command);
        GpuCommandBufferFlush(&Command);
        
        GpuBufferFree(&Temp);
    }
}
