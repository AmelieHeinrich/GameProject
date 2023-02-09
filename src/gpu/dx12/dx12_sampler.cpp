/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 00:32
 */

#include "dx12_sampler.hpp"

#include "dx12_context.hpp"

D3D12_TEXTURE_ADDRESS_MODE Dx12AddressMode(gpu_texture_address Address)
{
    switch (Address)
    {
        case gpu_texture_address::Border:
            return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        case gpu_texture_address::Clamp:
            return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case gpu_texture_address::Mirror:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        case gpu_texture_address::Wrap:
            return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }
}

D3D12_FILTER Dx12Filter(gpu_texture_filter Filter)
{
    switch (Filter)
    {
        case gpu_texture_filter::Anisotropic:
            return D3D12_FILTER_ANISOTROPIC;
        case gpu_texture_filter::Linear:
            return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        case gpu_texture_filter::Nearest:
            return D3D12_FILTER_MIN_MAG_MIP_POINT;
    }
}

void GpuSamplerInit(gpu_sampler *Sampler, gpu_texture_address Address, gpu_texture_filter Filter)
{
    Sampler->Address = Address;
    Sampler->Filter = Filter;
    Sampler->Private = new dx12_sampler;
    dx12_sampler *Private = (dx12_sampler*)Sampler->Private;

    Private->Desc.AddressU = Dx12AddressMode(Address);
    Private->Desc.AddressV = Private->Desc.AddressU;
    Private->Desc.AddressW = Private->Desc.AddressV;
    Private->Desc.MaxLOD = D3D12_FLOAT32_MAX;
    Private->Desc.MaxAnisotropy = 16;
    Private->Desc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    Private->Desc.Filter = Dx12Filter(Filter);

    Private->Descriptor = Dx12DescriptorHeapAlloc(&DX12.SamplerHeap);
    DX12.Device->CreateSampler(&Private->Desc, Dx12DescriptorHeapCPU(&DX12.SamplerHeap, Private->Descriptor));
}

void GpuSamplerFree(gpu_sampler *Sampler)
{
    dx12_sampler *Private = (dx12_sampler*)Sampler->Private;
    Dx12DescriptorHeapFreeSpace(&DX12.SamplerHeap, Private->Descriptor);
}
