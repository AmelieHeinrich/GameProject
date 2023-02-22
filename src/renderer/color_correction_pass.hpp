/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 20/02/2023 18:51
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

struct color_correction_pass
{
    gpu_image *HDRImage;
    gpu_pipeline Pipeline;
};

void ColorCorrectionPassInit(color_correction_pass *Pass, gpu_image *HDRImage);
void ColorCorrectionPassExit(color_correction_pass *Pass);
void ColorCorrectionPassUpdate(color_correction_pass *Pass, gpu_buffer *Settings);
void ColorCorrectionPassResize(color_correction_pass *Pass, uint32_t Width, uint32_t Height, gpu_image *HDRImage);