/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:58
 */

#pragma once

#include "gpu/gpu_buffer.hpp"

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

struct vulkan_buffer
{
    VkBuffer Buffer;
    VmaAllocation Allocation;
};
