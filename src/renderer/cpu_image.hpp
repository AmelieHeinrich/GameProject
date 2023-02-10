/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 11:48
 */

#pragma once

#include <cstdint>
#include <string>

struct cpu_image
{
    int Width;
    int Height;
    int Channels;
    bool Float;
    void *Data;
};

void CpuImageLoad(cpu_image* Image, const std::string& Path);
void CpuImageInitColor(cpu_image *Image, uint32_t Width, uint32_t Height, uint32_t Color);
void CpuImageFree(cpu_image *Image);
