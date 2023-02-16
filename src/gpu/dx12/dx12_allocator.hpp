/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 13:03
 */

#pragma once

#include "systems/allocator_system.hpp"
#include <d3d12.h>

enum class dx12_heap_type
{
    Default = D3D12_HEAP_TYPE_DEFAULT,
    Upload = D3D12_HEAP_TYPE_UPLOAD,
    Readback = D3D12_HEAP_TYPE_READBACK
};

struct dx12_resource
{
    ID3D12Resource *Resource;
    uint64_t Offset;
    uint64_t Size;
};

struct dx12_heap
{
    ID3D12Heap *Heap;
    linear_allocator Allocator;
    dx12_heap_type Type;
};

void Dx12HeapInit(dx12_heap *Heap, uint64_t Size, dx12_heap_type Type);
void Dx12HeapFree(dx12_heap *Heap);
void Dx12HeapAlloc(dx12_heap *Heap, D3D12_RESOURCE_DESC *ResourceDesc, D3D12_RESOURCE_STATES State, dx12_resource *Resource);
void Dx12HeapFreeBlock(dx12_heap *Heap, dx12_resource *Resource);
void Dx12HeapReset(dx12_heap *Heap);
