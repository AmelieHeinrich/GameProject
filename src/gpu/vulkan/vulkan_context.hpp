/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:55
 */

#pragma once

#include "gpu/gpu_context.hpp"

#define VK_USE_PLATFORM_WIN32_KHR 1
#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

#include "math_types.hpp"
#include "gpu/gpu_command_buffer.hpp"

struct vulkan_context
{
    uint32_t Width;
    uint32_t Height;

    VkInstance Instance;
    VkSurfaceKHR Surface;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;

    uint32_t GraphicsQueueFamily;
    VkQueue GraphicsQueue;
    VkCommandPool GraphicsPool;
    VkFence GraphicsFence;

    uint32_t ComputeQueueFamily;
    VkQueue ComputeQueue;
    VkCommandPool ComputePool;
    VkFence ComputeFence;

    uint32_t UploadQueueFamily;
    VkQueue UploadQueue;
    VkCommandPool UploadPool;
    VkFence UploadFence;
};

extern vulkan_context VK;
