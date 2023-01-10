/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 15:08
 */

#include "dx11_sampler.hpp"
#include "dx11_context.hpp"

#include "systems/log_system.hpp"

void GpuSamplerInit(gpu_sampler *Sampler, gpu_texture_address Address)
{
    Sampler->Address = Address;

    D3D11_SAMPLER_DESC Desc = {};
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    Desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)Address;
    Desc.AddressV = Desc.AddressU;
    Desc.AddressW = Desc.AddressV;
    Desc.MipLODBias = 0.0f;
    Desc.MaxAnisotropy = 1;
    Desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    Desc.MinLOD = 0;
    Desc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT Result = DxRenderContext.Device->CreateSamplerState(&Desc, &Sampler->Sampler);
    if (FAILED(Result))
        LogError("Failed to create sampler state!");
}

void GpuSamplerFree(gpu_sampler *Sampler)
{
    SafeRelease(Sampler->Sampler);
}

void GpuSampelrBind(gpu_sampler *Sampler, int Binding, gpu_resource_bind Bind)
{
    switch (Bind) {
        case gpu_resource_bind::Vertex: {
            DxRenderContext.DeviceContext->VSSetSamplers(Binding, 1, &Sampler->Sampler);
            return; 
        }
        case gpu_resource_bind::Pixel: {
            DxRenderContext.DeviceContext->PSSetSamplers(Binding, 1, &Sampler->Sampler);
            return; 
        }
        case gpu_resource_bind::Compute: {
            DxRenderContext.DeviceContext->CSSetSamplers(Binding, 1, &Sampler->Sampler);
            return;
        }
    }
}

