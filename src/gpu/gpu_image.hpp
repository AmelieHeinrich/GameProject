/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 10:28
 */

#pragma once

#include <cstdint>

#include "renderer/cpu_image.hpp"

enum class gpu_image_format
{
    RGBA8,
    RGBA32Float,
    R32Depth
};

enum class gpu_image_layout
{
    ImageLayoutCommon,
    ImageLayoutShaderResource,
    ImageLayoutStorage,
    ImageLayoutDepth,
    ImageLayoutRenderTarget,
    ImageLayoutCopySource,
    ImageLayoutCopyDest,
    ImageLayoutPresent
};

typedef gpu_image_layout gpu_buffer_layout;

enum class gpu_image_usage
{
    ImageUsageRenderTarget,
    ImageUsageDepthTarget,
    ImageUsageStorage,
    ImageUsageShaderResource
};

struct gpu_image
{
    uint32_t Width;
    uint32_t Height;
    gpu_image_format Format;
    gpu_image_layout Layout;
    gpu_image_usage Usage;
    void *Private;
};

void GpuImageInit(gpu_image *Image, uint32_t Width, uint32_t Height, gpu_image_format Format, gpu_image_usage Usage);
void GpuImageInitCubeMap(gpu_image *Image, uint32_t Width, uint32_t Height, gpu_image_format Format);
void GpuImageInitFromCPU(gpu_image *Image, cpu_image *CPU);
void GpuImageFree(gpu_image *Image);
