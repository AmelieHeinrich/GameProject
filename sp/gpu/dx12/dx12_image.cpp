/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/02/2023 10:47
 */

#include "dx12_image.hpp"

#include <d3dx12/d3dx12.h>

#include "dx12_context.hpp"
#include "dx12_command_buffer.hpp"
#include "gpu/gpu_command_buffer.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

DXGI_FORMAT GetDXGIFormat(gpu_image_format Format)
{
    switch (Format)
    {
        case gpu_image_format::RGBA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case gpu_image_format::RGBA32Float:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case gpu_image_format::R32Depth:
            return DXGI_FORMAT_D32_FLOAT;
    }
}

D3D12_RESOURCE_FLAGS GetResourceFlag(gpu_image_usage Usage)
{
    switch (Usage)
    {
        case gpu_image_usage::ImageUsageRenderTarget:
            return D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        case gpu_image_usage::ImageUsageDepthTarget:
            return D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        case gpu_image_usage::ImageUsageStorage:
            return D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        default:
            return D3D12_RESOURCE_FLAG_NONE;
    }
}

void GpuImageInit(gpu_image *Image, uint32_t Width, uint32_t Height, gpu_image_format Format, gpu_image_usage Usage)
{
    Image->Width = Width;
    Image->Height = Height;
    Image->Format = Format;
    Image->Usage = Usage;
    Image->Private = new dx12_image;
    dx12_image *Private = (dx12_image*)Image->Private;

    switch (Usage)
    {
        case gpu_image_usage::ImageUsageRenderTarget:
            Image->Layout = gpu_image_layout::ImageLayoutRenderTarget;
            Private->State = D3D12_RESOURCE_STATE_RENDER_TARGET;
            break;
        case gpu_image_usage::ImageUsageDepthTarget:
            Image->Layout = gpu_image_layout::ImageLayoutDepth;
            Private->State = D3D12_RESOURCE_STATE_DEPTH_WRITE;
            break;
        case gpu_image_usage::ImageUsageShaderResource:
            Image->Layout = gpu_image_layout::ImageLayoutShaderResource;
            Private->State = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
            break;
        case gpu_image_usage::ImageUsageStorage:
            Image->Layout = gpu_image_layout::ImageLayoutStorage;
            Private->State = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            break;
    }

    D3D12_HEAP_PROPERTIES HeapProperties = {};
    HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC ResourceDesc = {};
    ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    ResourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    ResourceDesc.Width = Width;
    ResourceDesc.Height = Height;
    ResourceDesc.DepthOrArraySize = 1;
    ResourceDesc.MipLevels = 1;
    ResourceDesc.Format = GetDXGIFormat(Format);
    ResourceDesc.SampleDesc.Count = 1;
    ResourceDesc.SampleDesc.Quality = 0;
    ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    ResourceDesc.Flags = GetResourceFlag(Usage);

    HRESULT Result = DX12.Device->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, Private->State, nullptr, IID_PPV_ARGS(&Private->Resource));
    if (FAILED(Result))
        LogError("D3D12: Failed to create GPU image!");

    switch (Usage)
    {
        case gpu_image_usage::ImageUsageRenderTarget:
        {
            Private->RTV = Dx12DescriptorHeapAlloc(&DX12.RTVHeap);
            
            D3D12_RENDER_TARGET_VIEW_DESC Desc = {};
            Desc.Format = ResourceDesc.Format;
            Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            DX12.Device->CreateRenderTargetView(Private->Resource, &Desc, Dx12DescriptorHeapCPU(&DX12.RTVHeap, Private->RTV));
            break;
        }
        case gpu_image_usage::ImageUsageDepthTarget:
        {
            Private->DSV = Dx12DescriptorHeapAlloc(&DX12.DSVHeap);

            D3D12_DEPTH_STENCIL_VIEW_DESC Desc = {};
            Desc.Format = ResourceDesc.Format;
            Desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            DX12.Device->CreateDepthStencilView(Private->Resource, &Desc, Dx12DescriptorHeapCPU(&DX12.DSVHeap, Private->DSV));
            break;
        }
        case gpu_image_usage::ImageUsageShaderResource:
        {
            Private->SRV_UAV = Dx12DescriptorHeapAlloc(&DX12.CBVSRVUAVHeap);

            D3D12_SHADER_RESOURCE_VIEW_DESC Desc = {};
            Desc.Format = ResourceDesc.Format;
            Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            Desc.Texture2D.MipLevels = 1;
            DX12.Device->CreateShaderResourceView(Private->Resource, &Desc, Dx12DescriptorHeapCPU(&DX12.CBVSRVUAVHeap, Private->SRV_UAV));
            break;
        }
        case gpu_image_usage::ImageUsageStorage:
        {
            Private->SRV_UAV = Dx12DescriptorHeapAlloc(&DX12.CBVSRVUAVHeap);

            D3D12_UNORDERED_ACCESS_VIEW_DESC Desc = {};
            Desc.Format = ResourceDesc.Format;
            Desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
            DX12.Device->CreateUnorderedAccessView(Private->Resource, nullptr, &Desc, Dx12DescriptorHeapCPU(&DX12.CBVSRVUAVHeap, Private->SRV_UAV));
            break;
        }
    }
}

void GpuImageInitFromCPU(gpu_image *Image, cpu_image *CPU)
{
    GpuImageInit(Image, CPU->Width, CPU->Height, CPU->Float ? gpu_image_format::RGBA32Float : gpu_image_format::RGBA8, gpu_image_usage::ImageUsageShaderResource);
    
    float SizeType = CPU->Float ? 4 : 1;

    gpu_buffer Temp;
    GpuBufferInit(&Temp, CPU->Width * CPU->Height * SizeType * 4, 0, gpu_buffer_type::Vertex);
    GpuBufferUpload(&Temp, CPU->Data, CPU->Width * CPU->Height * SizeType * 4);

    gpu_command_buffer CommandBuffer;
    GpuCommandBufferInit(&CommandBuffer, gpu_command_buffer_type::Graphics);
    GpuCommandBufferBegin(&CommandBuffer);
    GpuCommandBufferImageBarrier(&CommandBuffer, Image, gpu_image_layout::ImageLayoutCopyDest);
    GpuCommandBufferBufferBarrier(&CommandBuffer, &Temp, gpu_buffer_layout::ImageLayoutCommon, gpu_buffer_layout::ImageLayoutCopySource);
    GpuCommandBufferCopyBufferToTexture(&CommandBuffer, &Temp, Image);
    GpuCommandBufferImageBarrier(&CommandBuffer, Image, gpu_image_layout::ImageLayoutShaderResource);
    GpuCommandBufferEnd(&CommandBuffer);
    GpuCommandBufferFlush(&CommandBuffer);
    GpuCommandBufferFree(&CommandBuffer);

    GpuBufferFree(&Temp);
}

void GpuImageFree(gpu_image *Image)
{
    dx12_image *Private = (dx12_image*)Image->Private;

    switch (Image->Usage)
    {
        case gpu_image_usage::ImageUsageRenderTarget:
            Dx12DescriptorHeapFreeSpace(&DX12.RTVHeap, Private->RTV);
            break;
        case gpu_image_usage::ImageUsageDepthTarget:
            Dx12DescriptorHeapFreeSpace(&DX12.DSVHeap, Private->DSV);
            break;
        case gpu_image_usage::ImageUsageShaderResource:
        case gpu_image_usage::ImageUsageStorage:
            Dx12DescriptorHeapFreeSpace(&DX12.CBVSRVUAVHeap, Private->SRV_UAV);
            break;
    }

    SafeRelease(Private->Resource);
    delete Image->Private;
}
