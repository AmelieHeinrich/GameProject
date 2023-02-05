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

    D3D12_HEAP_PROPERTIES HeapProperties = {};
    HeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
    HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProperties.CreationNodeMask = 0;
    HeapProperties.VisibleNodeMask = 0;

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

    HRESULT Result = DX12.Device->CreateCommittedResource(&HeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Private->Resource));
    if (FAILED(Result))
        LogError("Failed to create buffer of size %d!", Size);

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
    void *Pointer;
    HRESULT Result = Private->Resource->Map(0, nullptr, &Pointer);
    if (FAILED(Result))
        LogError("D3D12: Failed to map buffer in range (0, %d)", Size);
    memcpy(Pointer, Data, Size);
    Private->Resource->Unmap(0, nullptr);
}
