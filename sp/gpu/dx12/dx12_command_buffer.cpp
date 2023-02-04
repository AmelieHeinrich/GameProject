/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:39
 */

#include "gpu/gpu_command_buffer.hpp"

#include <d3d12.h>

#include "dx12_context.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

struct dx12_command_buffer
{
    ID3D12CommandAllocator *Allocator;
    ID3D12GraphicsCommandList *List;
};

D3D12_COMMAND_LIST_TYPE Dx12CommandBufferType(gpu_command_buffer_type Type)
{
    switch (Type)
    {
        case gpu_command_buffer_type::Graphics:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case gpu_command_buffer_type::Compute:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case gpu_command_buffer_type::Upload:
            return D3D12_COMMAND_LIST_TYPE_COPY;
    }
}

void GpuCommandBufferInit(gpu_command_buffer *Buffer, gpu_command_buffer_type Type)
{
    Buffer->Type = Type;
    Buffer->Private = (void*)(new dx12_command_buffer);

    dx12_command_buffer *Private = (dx12_command_buffer*)Buffer->Private;

    HRESULT Result = DX12.Device->CreateCommandAllocator(Dx12CommandBufferType(Type), IID_PPV_ARGS(&Private->Allocator));
    if (FAILED(Result))
        LogError("D3D12: Failed to create command allocator!");
    
    Result = DX12.Device->CreateCommandList(0, Dx12CommandBufferType(Type), Private->Allocator, nullptr, IID_PPV_ARGS(&Private->List));
    if (FAILED(Result))
        LogError("D3D12: Failed to create command list!");
}

void GpuCommandBufferFree(gpu_command_buffer *Buffer)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Buffer->Private;
    SafeRelease(Private->List);
    SafeRelease(Private->Allocator);

    delete Buffer->Private;
}
