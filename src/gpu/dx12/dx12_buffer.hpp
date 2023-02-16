/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 05/02/2023 21:58
 */

#pragma once

#include "gpu/gpu_buffer.hpp"

#include <d3d12.h>
#include <D3D12MA/D3D12MemAlloc.h>

struct dx12_buffer
{
    ID3D12Resource* Resource;
    D3D12MA::Allocation *Allocation;

    int HeapIndex;
    D3D12_VERTEX_BUFFER_VIEW VertexView;
    D3D12_INDEX_BUFFER_VIEW IndexView;
    D3D12_CONSTANT_BUFFER_VIEW_DESC ConstantDesc;
    D3D12_UNORDERED_ACCESS_VIEW_DESC UnorderedDesc;
};
