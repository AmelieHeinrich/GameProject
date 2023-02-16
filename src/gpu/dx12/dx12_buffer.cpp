/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 21:10
 */

#include "dx12_buffer.hpp"

#include "dx12_context.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

void GpuBufferInit(gpu_buffer *Buffer, uint64_t Size, uint64_t Stride, gpu_buffer_type Type)
{
    Buffer->Size = Size;
    Buffer->Stride = Stride;
    Buffer->Type = Type;
    Buffer->Reserved = (void*)(new dx12_buffer);

    dx12_buffer *Private = (dx12_buffer*)Buffer->Reserved;
    Private->HeapIndex = -1;

    D3D12MA::ALLOCATION_DESC AllocDesc = {};
    AllocDesc.HeapType = Type == gpu_buffer_type::Uniform ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC ResourceDesc = {};
    ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    ResourceDesc.Width = Size;
    ResourceDesc.Height = 1;
    ResourceDesc.DepthOrArraySize = 1;
    ResourceDesc.MipLevels = 1;
    ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    ResourceDesc.SampleDesc.Count = 1;
    ResourceDesc.SampleDesc.Quality = 0;
    ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT Result = DX12.Allocator->CreateResource(&AllocDesc, &ResourceDesc, Type == gpu_buffer_type::Uniform ? D3D12_RESOURCE_STATE_GENERIC_READ : D3D12_RESOURCE_STATE_COMMON, nullptr, &Private->Allocation, IID_PPV_ARGS(&Private->Resource));
    if (FAILED(Result))
        LogError("D3D12: Failed to allocate buffer of size %d!", Size);

    switch (Type)
    {
        case gpu_buffer_type::Vertex: {
            Private->VertexView.BufferLocation = Private->Resource->GetGPUVirtualAddress();
            Private->VertexView.SizeInBytes = Size;
            Private->VertexView.StrideInBytes = Stride;
        } break;
        case gpu_buffer_type::Index: {
            Private->IndexView.BufferLocation = Private->Resource->GetGPUVirtualAddress();
            Private->IndexView.SizeInBytes = Size;
            Private->IndexView.Format = DXGI_FORMAT_R32_UINT;
        } break;
        case gpu_buffer_type::Uniform: {
            Private->ConstantDesc.BufferLocation = Private->Resource->GetGPUVirtualAddress();
            Private->ConstantDesc.SizeInBytes = Size;
            Private->HeapIndex = Dx12DescriptorHeapAlloc(&DX12.CBVSRVUAVHeap);
            DX12.Device->CreateConstantBufferView(&Private->ConstantDesc, Dx12DescriptorHeapCPU(&DX12.CBVSRVUAVHeap, Private->HeapIndex));
        } break;
        default: {
            LogWarn("D3D12: Unsupported buffer type!");
        } break;
    }
}

void GpuBufferInitForCopy(gpu_buffer *Buffer, uint64_t Size)
{
    Buffer->Size = Size;
    Buffer->Reserved = (void*)(new dx12_buffer);

    dx12_buffer *Private = (dx12_buffer*)Buffer->Reserved;
    Private->HeapIndex = -1;

    D3D12MA::ALLOCATION_DESC AllocDesc = {};
    AllocDesc.HeapType = D3D12_HEAP_TYPE_READBACK;

    D3D12_RESOURCE_DESC ResourceDesc = {};
    ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    ResourceDesc.Width = Size;
    ResourceDesc.Height = 1;
    ResourceDesc.DepthOrArraySize = 1;
    ResourceDesc.MipLevels = 1;
    ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    ResourceDesc.SampleDesc.Count = 1;
    ResourceDesc.SampleDesc.Quality = 0;
    ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT Result = DX12.Allocator->CreateResource(&AllocDesc, &ResourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, &Private->Allocation, IID_PPV_ARGS(&Private->Resource));
    if (FAILED(Result))
        LogError("D3D12: Failed to allocate buffer of size %d!", Size);
}

void GpuBufferInitForUpload(gpu_buffer *Buffer, uint64_t Size)
{
    Buffer->Size = Size;
    Buffer->Type = gpu_buffer_type::Uniform;
    Buffer->Reserved = (void*)(new dx12_buffer);

    dx12_buffer *Private = (dx12_buffer*)Buffer->Reserved;
    Private->HeapIndex = -1;

    D3D12MA::ALLOCATION_DESC AllocDesc = {};
    AllocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC ResourceDesc = {};
    ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    ResourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    ResourceDesc.Width = Size;
    ResourceDesc.Height = 1;
    ResourceDesc.DepthOrArraySize = 1;
    ResourceDesc.MipLevels = 1;
    ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    ResourceDesc.SampleDesc.Count = 1;
    ResourceDesc.SampleDesc.Quality = 0;
    ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT Result = DX12.Allocator->CreateResource(&AllocDesc, &ResourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, &Private->Allocation, IID_PPV_ARGS(&Private->Resource));
    if (FAILED(Result))
        LogError("D3D12: Failed to allocate buffer of size %d!", Size);
}

void GpuBufferFree(gpu_buffer *Buffer)
{
    dx12_buffer *Private = (dx12_buffer*)Buffer->Reserved;

    if (Private->HeapIndex != -1)
        Dx12DescriptorHeapFreeSpace(&DX12.CBVSRVUAVHeap, Private->HeapIndex);
    SafeRelease(Private->Resource);
    delete Private;
}

void GpuBufferUpload(gpu_buffer *Buffer, const void *Data, uint64_t Size)
{
    dx12_buffer *Private = (dx12_buffer*)Buffer->Reserved;
    
    if (Buffer->Type == gpu_buffer_type::Uniform)
    {
        void *Pointer;
        HRESULT Result = Private->Resource->Map(0, nullptr, &Pointer);
        if (FAILED(Result))
            LogError("D3D12: Failed to map buffer in range (0, %d)", Size);
        memcpy(Pointer, Data, Size);
        Private->Resource->Unmap(0, nullptr);
    }
    else
    {
        gpu_buffer Temp;
        GpuBufferInitForUpload(&Temp, Size);

        dx12_buffer *TempPrivate = (dx12_buffer*)Temp.Reserved;
        void *Pointer;
        HRESULT Result = TempPrivate->Resource->Map(0, nullptr, &Pointer);
        if (FAILED(Result))
            LogError("D3D12: Failed to map buffer in range (0, %d)", Size);
        memcpy(Pointer, Data, Size);
        TempPrivate->Resource->Unmap(0, nullptr);
    
        gpu_command_buffer Command;
        GpuCommandBufferInit(&Command, gpu_command_buffer_type::Graphics);
        GpuCommandBufferBegin(&Command);
        GpuCommandBufferBufferBarrier(&Command, &Temp, gpu_buffer_layout::ImageLayoutCommon, gpu_buffer_layout::ImageLayoutCopySource);
        GpuCommandBufferBufferBarrier(&Command, Buffer, gpu_buffer_layout::ImageLayoutCommon, gpu_buffer_layout::ImageLayoutCopyDest);
        GpuCommandBufferCopyBufferToBuffer(&Command, &Temp, Buffer);
        GpuCommandBufferBufferBarrier(&Command, Buffer, gpu_buffer_layout::ImageLayoutCopyDest, gpu_buffer_layout::ImageLayoutGenericRead);
        GpuCommandBufferEnd(&Command);
        GpuCommandBufferFlush(&Command);
        
        GpuBufferFree(&Temp);
    }
}
