/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 13:45
 */

#pragma once

#include <cstdint>
#include <d3d12.h>
#include <vector>

struct dx12_descriptor_heap
{
    ID3D12DescriptorHeap *Heap;
    uint32_t IncrementSize;
    uint32_t DescriptorCount;
    D3D12_DESCRIPTOR_HEAP_TYPE Type;
    std::vector<bool> DescriptorLookupTable;
};

void Dx12DescriptorHeapInit(dx12_descriptor_heap *Heap, D3D12_DESCRIPTOR_HEAP_TYPE Type, uint32_t Count);
void Dx12DescriptorHeapFree(dx12_descriptor_heap *Heap);
D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapCPU(dx12_descriptor_heap *Heap, int Offset);
D3D12_GPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapGPU(dx12_descriptor_heap *Heap, int Offset);
int Dx12DescriptorHeapAlloc(dx12_descriptor_heap *Heap);
void Dx12DescriptorHeapFreeSpace(dx12_descriptor_heap *Heap, int Descriptor);
