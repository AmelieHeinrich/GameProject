/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 15:04
 */

#pragma once

#include "dx11_texture.hpp"

enum class gpu_texture_address
{
    Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
    Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
    Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
    Border = D3D11_TEXTURE_ADDRESS_BORDER
};

struct gpu_sampler
{
    gpu_texture_address Address;
    ID3D11SamplerState* Sampler;
};

void GpuSamplerInit(gpu_sampler *Sampler, gpu_texture_address Address);
void GpuSamplerFree(gpu_sampler *Sampler);
void GpuSampelrBind(gpu_sampler *Sampler, int Binding, gpu_resource_bind Bind);
