/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 13:47
 */

#include "dx12_descriptor_heap.hpp"

#include "dx12_context.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

const char *HeapTypeToString(D3D12_DESCRIPTOR_HEAP_TYPE Type)
{
    switch (Type)
    {
        case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
            return "D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV";
        case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
            return "D3D12_DESCRIPTOR_HEAP_TYPE_DSV";
        case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
            return "D3D12_DESCRIPTOR_HEAP_TYPE_RTV";
        case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
            return "D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER";
    }
}

void Dx12DescriptorHeapInit(dx12_descriptor_heap *Heap, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t Count)
{
    Heap->Type = Type;
    Heap->DescriptorCount = Count;
    Heap->DescriptorLookupTable = std::vector<bool>(Count, false);

    D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
    Desc.NumDescriptors = Count;
    Desc.Type = Type;
    Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    if (Type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV || Type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
        Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    
    HRESULT Result = DX12.Device->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&Heap->Heap));
    if (FAILED(Result))
        LogError("D3D12: Failed to create descriptor heap!");
    Heap->IncrementSize = DX12.Device->GetDescriptorHandleIncrementSize(Type);

    LogDebug("D3D12: Allocated descriptor heap of type (%s) of size (%d)", HeapTypeToString(Type), Count);
}

void Dx12DescriptorHeapFree(dx12_descriptor_heap *Heap)
{
    SafeRelease(Heap->Heap);
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapCPU(dx12_descriptor_heap *Heap, int Offset)
{
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = Heap->Heap->GetCPUDescriptorHandleForHeapStart();
    Handle.ptr += Offset * Heap->IncrementSize;
    return Handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapGPU(dx12_descriptor_heap *Heap, int Offset)
{
    D3D12_GPU_DESCRIPTOR_HANDLE Handle = Heap->Heap->GetGPUDescriptorHandleForHeapStart();
    Handle.ptr += Offset * Heap->IncrementSize;
    return Handle;
}

int Dx12DescriptorHeapAlloc(dx12_descriptor_heap *Heap)
{
    for (int Descriptor = 0; Descriptor < Heap->DescriptorCount; Descriptor++) {
        if (Heap->DescriptorLookupTable[Descriptor] == false) {
            Heap->DescriptorLookupTable[Descriptor] = true;
            return Descriptor;
        }
    }
    return -1;
}

void Dx12DescriptorHeapFreeSpace(dx12_descriptor_heap *Heap, int Descriptor)
{
    Heap->DescriptorLookupTable[Descriptor] = false;
}
