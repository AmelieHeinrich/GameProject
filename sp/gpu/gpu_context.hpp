/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 11:30
 */

#pragma once

#include <cstdint>
#include <cstdlib>

#include "gpu_command_buffer.hpp"
#include "gpu_image.hpp"
#include "math_types.hpp"

void GpuInit();
void GpuExit();
void GpuBeginFrame();
void GpuEndFrame();
void GpuResize(uint32_t Width, uint32_t Height);
void GpuPresent();
hmm_v2 GpuGetDimensions();
gpu_command_buffer* GpuGetImageCommandBuffer();
gpu_image* GpuGetSwapChainImage();
