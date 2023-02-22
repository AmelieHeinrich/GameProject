/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 20/02/2023 19:35
 */

#pragma once

#include "gpu/gpu_command_buffer.hpp"
#include "gpu/gpu_buffer.hpp"
#include "gpu/gpu_image.hpp"
#include "gpu/gpu_sampler.hpp"
#include "gpu/gpu_shader.hpp"
#include "gpu/gpu_pipeline.hpp"

struct sharpness_pass
{
    gpu_image *Image;

    gpu_pipeline Pipeline;
};

void SharpnessPassInit(sharpness_pass *Pass, gpu_image *Image);
void SharpnessPassExit(sharpness_pass *Pass);
void SharpnessPassUpdate(sharpness_pass *Pass, gpu_buffer *Settings);
void SharpnessPassResize(sharpness_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *Image);
