/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 09:55
 */

#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>

#include "math_types.hpp"
#include "gpu/gpu_command_buffer.hpp"

struct vulkan_context
{
    uint32_t Width;
    uint32_t Height;
};

extern vulkan_context VK;
