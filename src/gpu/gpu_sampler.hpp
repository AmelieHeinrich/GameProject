/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 00:30
 */

#pragma once

enum class gpu_texture_address
{
    Wrap,
    Mirror,
    Clamp,
    Border
};

enum class gpu_texture_filter
{
    Nearest,
    Linear,
    Anisotropic
};

struct gpu_sampler
{
    gpu_texture_address Address;
    gpu_texture_filter Filter;
    void *Private;
};

void GpuSamplerInit(gpu_sampler *Sampler, gpu_texture_address Address, gpu_texture_filter Filter);
void GpuSamplerFree(gpu_sampler *Sampler);
