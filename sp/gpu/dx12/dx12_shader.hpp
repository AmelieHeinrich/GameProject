/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 13:02
 */

#pragma once

#include <d3d12.h>

#include "gpu/gpu_shader.hpp"

struct dx12_shader
{
    ID3DBlob *VertexBlob;
    ID3DBlob *PixelBlob;
    ID3DBlob *ComputeBlob;
};
