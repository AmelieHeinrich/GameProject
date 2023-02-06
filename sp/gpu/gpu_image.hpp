/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 10:28
 */

#pragma once

#include <cstdint>

enum class gpu_image_format
{
    RGBA8,
    R32Depth
};

enum class gpu_image_layout
{
    ImageLayoutShaderResource,
    ImageLayoutStorage,
    ImageLayoutDepth,
    ImageLayoutRenderTarget,
    ImageLayoutCopySource,
    ImageLayoutCopyDest,
    ImageLayoutPresent
};

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
void GpuImageFree(gpu_image *Image);
