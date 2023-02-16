/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 13:05
 */

#include "dx12_allocator.hpp"

#include "dx12_context.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

void Dx12HeapInit(dx12_heap *Heap, uint64_t Size, dx12_heap_type Type)
{
    Heap->Type = Type;
    LinearAllocatorInit(&Heap->Allocator, Size);

    D3D12_HEAP_DESC Desc = {};
    Desc.SizeInBytes = Size;
    Desc.Properties.Type = (D3D12_HEAP_TYPE)Type;
    Desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    
    HRESULT Result = DX12.Device->CreateHeap(&Desc, IID_PPV_ARGS(&Heap->Heap));
    if (FAILED(Result))
        LogError("D3D12: Failed to create GPU heap of size %d", Size);
}

void Dx12HeapFree(dx12_heap *Heap)
{
    SafeRelease(Heap->Heap);
}

void Dx12HeapAlloc(dx12_heap *Heap, D3D12_RESOURCE_DESC *ResourceDesc, D3D12_RESOURCE_STATES State, dx12_resource *Resource)
{
    Resource->Size = ResourceDesc->Width * ResourceDesc->Height * ResourceDesc->DepthOrArraySize;
    Resource->Offset = LinearAllocatorAlloc(&Heap->Allocator, Resource->Size);

    HRESULT Result = DX12.Device->CreatePlacedResource(Heap->Heap, Resource->Offset, ResourceDesc, State, nullptr, IID_PPV_ARGS(&Resource->Resource));
    if (FAILED(Result))
        LogError("D3D12: Failed to allocate memory block! (Offset: %d/%d)", Resource->Offset, Heap->Allocator.Size);
}

void Dx12HeapFreeBlock(dx12_heap *Heap, dx12_resource *Resource)
{
    SafeRelease(Resource->Resource);
}
