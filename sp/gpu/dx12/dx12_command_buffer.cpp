/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 03/02/2023 11:39
 */

#include "dx12_command_buffer.hpp"

#include <d3d12.h>

#include "dx12_buffer.hpp"
#include "dx12_context.hpp"
#include "dx12_image.hpp"
#include "dx12_pipeline.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

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

D3D12_RESOURCE_STATES GetStateFromImageLayout(gpu_image_layout Layout)
{
    switch (Layout)
    {
        case gpu_image_layout::ImageLayoutCommon:
            return D3D12_RESOURCE_STATE_COMMON;
        case gpu_image_layout::ImageLayoutCopyDest:
            return D3D12_RESOURCE_STATE_COPY_DEST;
        case gpu_image_layout::ImageLayoutCopySource:
            return D3D12_RESOURCE_STATE_COPY_SOURCE;
        case gpu_image_layout::ImageLayoutDepth:
            return D3D12_RESOURCE_STATE_DEPTH_WRITE;
        case gpu_image_layout::ImageLayoutPresent:
            return D3D12_RESOURCE_STATE_PRESENT;
        case gpu_image_layout::ImageLayoutRenderTarget:
            return D3D12_RESOURCE_STATE_RENDER_TARGET;
        case gpu_image_layout::ImageLayoutShaderResource:
            return D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
        case gpu_image_layout::ImageLayoutStorage:
            return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
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
            break;
        default:
            LogWarn("D3D12: Trying to bind a buffer that isn't gpu_buffer_type::Vertex or gpu_buffer_type::Index!");
            break;
    }
}

void GpuCommandBufferBindPipeline(gpu_command_buffer *Command, gpu_pipeline *Pipeline)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_pipeline *PipelinePrivate = (dx12_pipeline*)Pipeline->Private;

    Private->List->SetPipelineState(PipelinePrivate->Pipeline);
    switch (Pipeline->Info.Type)
    {
        case gpu_pipeline_type::Compute:
            Private->List->SetComputeRootSignature(PipelinePrivate->Signature);
            break;
        case gpu_pipeline_type::Graphics:
            Private->List->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            Private->List->SetGraphicsRootSignature(PipelinePrivate->Signature);
            break;
    }
}

void GpuCommandBufferBindConstantBuffer(gpu_command_buffer *Command, gpu_pipeline_type Type, gpu_buffer *Buffer, int Offset)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_buffer *BufferPrivate = (dx12_buffer*)Buffer->Reserved;

    switch (Type)
    {
        case gpu_pipeline_type::Graphics:
            Private->List->SetGraphicsRootConstantBufferView(Offset, BufferPrivate->Resource->GetGPUVirtualAddress());
            break;
        case gpu_pipeline_type::Compute:
            Private->List->SetComputeRootConstantBufferView(Offset, BufferPrivate->Resource->GetGPUVirtualAddress());
            break;
    }
}

void GpuCommandBufferBindRenderTarget(gpu_command_buffer *Command, gpu_image *Image, gpu_image *Depth)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_image *ImagePrivate = (dx12_image*)Image->Private;

    D3D12_CPU_DESCRIPTOR_HANDLE RTV = Dx12DescriptorHeapCPU(&DX12.RTVHeap, ImagePrivate->RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE DSV;

    if (Depth)
    {
        dx12_image *DepthPrivate = (dx12_image*)Depth->Private;
        DSV = Dx12DescriptorHeapCPU(&DX12.DSVHeap, DepthPrivate->DSV);
    }

    Private->List->OMSetRenderTargets(1, &RTV, false, Depth != nullptr ? &DSV : nullptr);
}

void GpuCommandBufferClearColor(gpu_command_buffer *Command, gpu_image *Image, float Red, float Green, float Blue, float Alpha)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_image *ImagePrivate = (dx12_image*)Image->Private;

    float Clear[4] = { Red, Green, Blue, Alpha };

    auto CPUHandle = Dx12DescriptorHeapCPU(&DX12.RTVHeap, ImagePrivate->RTV);
    Private->List->ClearRenderTargetView(CPUHandle, Clear, 0, nullptr);
}

void GpuCommandBufferClearDepth(gpu_command_buffer *Command, gpu_image *Image, float Depth, float Stencil)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_image *ImagePrivate = (dx12_image*)Image->Private;

    auto CPUHandle = Dx12DescriptorHeapCPU(&DX12.DSVHeap, ImagePrivate->DSV);
    Private->List->ClearDepthStencilView(CPUHandle, D3D12_CLEAR_FLAG_DEPTH, Depth, Stencil, 0, nullptr);
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

    D3D12_RECT Rect;
    Rect.left = 0;
    Rect.top = 0;
    Rect.right = Width;
    Rect.bottom = Height;

    Private->List->RSSetViewports(1, &Viewport);
    Private->List->RSSetScissorRects(1, &Rect);
}

void GpuCommandBufferDraw(gpu_command_buffer *Command, int VertexCount)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    Private->List->DrawInstanced(VertexCount, 1, 0, 0);
}

void GpuCommandBufferDrawIndexed(gpu_command_buffer *Command, int IndexCount)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    Private->List->DrawIndexedInstanced(IndexCount, 1, 0, 0, 0);
}

void GpuCommandBufferDispatch(gpu_command_buffer *Command, int X, int Y, int Z)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    Private->List->Dispatch(X, Y, Z);
}

void GpuCommandBufferImageBarrier(gpu_command_buffer *Command, gpu_image *Image, gpu_image_layout New)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_image *ImagePrivate = (dx12_image*)Image->Private;

    D3D12_RESOURCE_BARRIER Barrier = {};
    Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    Barrier.Transition.pResource = ImagePrivate->Resource;
    Barrier.Transition.StateBefore = GetStateFromImageLayout(Image->Layout);
    Barrier.Transition.StateAfter = GetStateFromImageLayout(New);
    Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    Image->Layout = New;
    ImagePrivate->State = GetStateFromImageLayout(Image->Layout);

    Private->List->ResourceBarrier(1, &Barrier);
}

void GpuCommandBufferBlit(gpu_command_buffer *Command, gpu_image *Source, gpu_image *Dest)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    dx12_image *SourcePrivate = (dx12_image*)Source->Private;
    dx12_image *DestPrivate = (dx12_image*)Dest->Private;

    D3D12_TEXTURE_COPY_LOCATION BlitSource = {};
    BlitSource.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    BlitSource.pResource = SourcePrivate->Resource;
    BlitSource.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION BlitDest = {};
    BlitDest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    BlitDest.pResource = DestPrivate->Resource;
    BlitDest.SubresourceIndex = 0;

    Private->List->CopyTextureRegion(&BlitDest, 0, 0, 0, &BlitSource, nullptr);
}

void GpuCommandBufferBegin(gpu_command_buffer *Command)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    Private->Allocator->Reset();
    Private->List->Reset(Private->Allocator, nullptr);
}

void GpuCommandBufferEnd(gpu_command_buffer *Command)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;
    
    Private->List->Close();
}

void GpuCommandBufferFlush(gpu_command_buffer *Command)
{
    dx12_command_buffer *Private = (dx12_command_buffer*)Command->Private;

    ID3D12CommandList* CommandLists[] = { Private->List };
    DX12.CommandQueue->ExecuteCommandLists(1, CommandLists);

    Dx12FenceFlush(&DX12.DeviceFence);
}
