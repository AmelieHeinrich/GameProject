/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 17:51
 */

#pragma once

#include "gpu/gpu_buffer.hpp"
#include "gpu/gpu_image.hpp"
#include "gpu/gpu_sampler.hpp"
#include "gpu/gpu_shader.hpp"
#include "gpu/gpu_pipeline.hpp"
#include "cpu_image.hpp"

#include "scene/scene.hpp"
#include "mesh.hpp"

struct tonemapping_pass
{
    gpu_image *HDRImage;
    gpu_image LDRImage;

    gpu_pipeline Pipeline;
};

void TonemappingPassInit(tonemapping_pass *Pass, gpu_image *HDRImage);
void TonemappingPassExit(tonemapping_pass *Pass);
void TonemappingPassUpdate(tonemapping_pass *Pass, gpu_buffer *Settings);
void TonemappingPassResize(tonemapping_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *HDRImage);
