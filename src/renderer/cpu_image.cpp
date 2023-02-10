/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 11:49
 */

#include "cpu_image.hpp"

#include <stb/stb_image.h>
#include "systems/log_system.hpp"

void CpuImageLoad(cpu_image* Image, const std::string& Path)
{
    std::string Extension = Path.substr(Path.find_last_of(".") + 1);
    if (Extension != "hdr")
    {
        Image->Data = stbi_load(Path.c_str(), &Image->Width, &Image->Height, &Image->Channels, STBI_rgb_alpha);
        Image->Float = false;
    }
    else
    {
        Image->Data = stbi_loadf(Path.c_str(), &Image->Width, &Image->Height, &Image->Channels, STBI_rgb_alpha);
        Image->Float = true;
    }
    if (!Image->Data)
        LogError("CpuImage: Failed to load image at path %s", Path.c_str());
}

void CpuImageInitColor(cpu_image *Image, uint32_t Width, uint32_t Height, uint32_t Color)
{
    Image->Width = Width;
    Image->Height = Height;
    Image->Channels = 4;
    Image->Data = new char[Width * Height];
    memset(Image->Data, Color, Width * Height);
}

void CpuImageFree(cpu_image *Image)
{
    stbi_image_free(Image->Data);
}
