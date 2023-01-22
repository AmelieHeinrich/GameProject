/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/01/2023 10:31
 */

#pragma once

#include "gpu/dx11_buffer.hpp"
#include "gpu/dx11_texture.hpp"

struct forward_pass
{
    gpu_texture RenderTexture;
    gpu_texture DepthTexture;
};

void ForwardPassInit(forward_pass *Pass);
void ForwardPassFree(forward_pass *Pass);
void ForwardPassExecute(forward_pass *Pass);
void ForwardPassResize(forward_pass *Pass, uint32_t Width, uint32_t Height);
