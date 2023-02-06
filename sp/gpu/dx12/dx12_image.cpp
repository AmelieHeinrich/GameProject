/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 10:47
 */

#include "dx12_image.hpp"

void GpuImageInit(gpu_image *Image, uint32_t Width, uint32_t Height, gpu_image_format Format)
{
    Image->Private = new dx12_image;
}

void GpuImageFree(gpu_image *Image)
{
    delete Image->Private;
}
