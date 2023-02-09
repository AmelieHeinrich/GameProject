/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 16:31
 */

#pragma once

#include "gpu/gpu_buffer.hpp"
#include "gpu/gpu_image.hpp"
#include "gpu/gpu_shader.hpp"
#include "gpu/gpu_pipeline.hpp"
#include "cpu_image.hpp"

#include "scene/scene.hpp"

struct forward_pass
{
    gpu_image RenderTarget;
    gpu_image DepthTarget;

    gpu_buffer CameraBuffer;
    gpu_pipeline Pipeline;
    gpu_buffer TriangleBuffer;
};

void ForwardPassInit(forward_pass *Pass);
void ForwardPassExit(forward_pass *Pass);
void ForwardPassUpdate(forward_pass *Pass, camera_data *Camera);
void ForwardPassResize(forward_pass *Pass, uint32_t Width, uint32_t Height);
