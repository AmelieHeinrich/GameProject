/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:39
 */

#include "gpu/gpu_command_buffer.hpp"

#include <d3d12.h>

#include "dx12_buffer.hpp"
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

    Result = Private->List->Close();
    if (FAILED(Result))
        LogError("D3D12: Failed to close command list!");
}

void GpuCommandBufferFree(gpu_command_buffer *Buffer)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Buffer->Private;
    SafeRelease(Private->List);
    SafeRelease(Private->Allocator);

    delete Buffer->Private;
}

void GpuCommandBufferBindBuffer(gpu_command_buffer *Command, gpu_buffer *Buffer)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_buffer *BufferPrivate = (dx12_buffer*)Buffer->Reserved;

    switch (Buffer->Type)
    {
        case gpu_buffer_type::Vertex:
            Private->List->IASetVertexBuffers(0, 1, &BufferPrivate->VertexView);
            break;
        case gpu_buffer_type::Index:
            Private->List->IASetIndexBuffer(&BufferPrivate->IndexView);
        default:
            LogWarn("D3D12: Trying to bind a buffer that isn't gpu_buffer_type::Vertex or gpu_buffer_type::Index!");
            break;
    }
}

void GpuCommandBufferSetViewport(gpu_command_buffer *Command, float Width, float Height, float X, float Y)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    D3D12_VIEWPORT Viewport = {};
    Viewport.Width = Width;
    Viewport.Height = Height;
    Viewport.TopLeftX = X;
    Viewport.TopLeftY = Y;
    Viewport.MinDepth = 0.0f;
    Viewport.MaxDepth = 1.0f;

    Private->List->RSSetViewports(1, &Viewport);
}
