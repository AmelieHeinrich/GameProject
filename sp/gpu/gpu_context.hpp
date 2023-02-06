/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 11:30
 */

#pragma once

#include <cstdint>

#include "gpu_command_buffer.hpp"
#include "gpu_image.hpp"

void GpuInit();
void GpuExit();
void GpuResize(uint32_t Width, uint32_t Height);
gpu_command_buffer* GpuGetImageCommandBuffer();
gpu_image* GpuGetSwapChainImage();
