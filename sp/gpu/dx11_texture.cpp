/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 12:44
 */

#include "dx11_texture.hpp"

void GpuTextureInit(gpu_texture *Texture, int Width, int Height, DXGI_FORMAT Format, gpu_texture_bind Bind)
{

}

void GpuTextureFree(gpu_texture *Texture)
{

}

void GpuTextureInitRTV(gpu_texture *Texture)
{

}

void GpuTextureInitDSV(gpu_texture *Texture, DXGI_FORMAT DepthFormat)
{

}

void GpuTextureInitSRV(gpu_texture *Texture)
{

}

void GpuTextureInitUAV(gpu_texture *Texture)
{

}

void GpuTextureBindRTV(gpu_texture *Texture, V4 ClearColor)
{

}

void GpuTextureBindSRV(gpu_texture *Texture, int Binding, gpu_resource_bind Bind)
{

}

void GpuTextureBindUAV(gpu_texture *Texture, int Binding)
{

}

void GpuResetRTV()
{

}

void GpuResetSRV(int Binding, gpu_resource_bind Bind)
{

}

void GpuResetUAV(int Binding)
{

}
