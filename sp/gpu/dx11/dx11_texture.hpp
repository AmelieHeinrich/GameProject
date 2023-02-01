/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 12:05
 */

#pragma once

#include "dx11_buffer.hpp"

#include "math_types.hpp"

enum class gpu_texture_bind
{
    RenderTarget = D3D11_BIND_RENDER_TARGET,
    DepthStencil = D3D11_BIND_DEPTH_STENCIL,
    ShaderResource = D3D11_BIND_SHADER_RESOURCE,
    UnorderedAccess = D3D11_BIND_UNORDERED_ACCESS
};

struct gpu_texture
{
    ID3D11Texture2D* Texture;
    int Width;
    int Height;

    DXGI_FORMAT Format;
    DXGI_FORMAT DepthFormat;

    ID3D11RenderTargetView* RTV;
    ID3D11DepthStencilView* DSV;
    ID3D11ShaderResourceView* SRV;
    ID3D11UnorderedAccessView* UAV;
};

void GpuTextureInit(gpu_texture *Texture, int Width, int Height, DXGI_FORMAT Format, gpu_texture_bind Bind);
void GpuTextureFree(gpu_texture *Texture);
void GpuTextureInitRTV(gpu_texture *Texture);
void GpuTextureInitDSV(gpu_texture *Texture, DXGI_FORMAT DepthFormat);
void GpuTextureInitSRV(gpu_texture *Texture);
void GpuTextureInitUAV(gpu_texture *Texture);

void GpuTextureBindRTV(gpu_texture *Texture, gpu_texture *Depth, V4 ClearColor);
void GpuTextureBindSRV(gpu_texture *Texture, int Binding, gpu_resource_bind Bind);
void GpuTextureBindUAV(gpu_texture *Texture, int Binding);

void GpuResetRTV();
void GpuResetSRV(int Binding, gpu_resource_bind Bind);
void GpuResetUAV(int Binding);
