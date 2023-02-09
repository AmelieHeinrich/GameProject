/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 14:42
 */

#pragma once

#include <cstdint>
#include <d3d12.h>

struct dx12_fence
{
    ID3D12Fence *Fence;
    uint64_t Value;
};

void Dx12FenceInit(dx12_fence *Fence);
void Dx12FenceFree(dx12_fence *Fence);
uint64_t Dx12FenceSignal(dx12_fence *Fence, ID3D12CommandQueue *Queue);
bool Dx12FenceReached(dx12_fence *Fence, uint64_t Value);
void Dx12FenceSync(dx12_fence *Fence, uint64_t Value);
void Dx12FenceFlush(dx12_fence *Fence, ID3D12CommandQueue *Queue);
