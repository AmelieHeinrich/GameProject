/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/01/2023 22:33
 */

#include "dx11_buffer.hpp"

#include "dx11_context.hpp"
#include "systems/log_system.hpp"

D3D11_BIND_FLAG GpuUsageToD3D11(gpu_buffer_usage Usage)
{
    switch (Usage)
    {
        case gpu_buffer_usage::Vertex:
            return D3D11_BIND_VERTEX_BUFFER;
        case gpu_buffer_usage::Index:
            return D3D11_BIND_INDEX_BUFFER;
        case gpu_buffer_usage::Constant:
            return D3D11_BIND_CONSTANT_BUFFER;
        case gpu_buffer_usage::UnorderedAccess:
            return D3D11_BIND_UNORDERED_ACCESS;
    }
    return D3D11_BIND_VERTEX_BUFFER;
}

void GpuBufferCreate(gpu_buffer *Buffer, int64_t Size, int64_t Stride, gpu_buffer_usage Usage)
{
    Buffer->Stride = Stride;

    D3D11_BUFFER_DESC Desc = {};
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.ByteWidth = Size;
    Desc.BindFlags = GpuUsageToD3D11(Usage);
    Desc.CPUAccessFlags = 0;
    Desc.MiscFlags = 0;

    HRESULT Result = DxRenderContext.Device->CreateBuffer(&Desc, nullptr, &Buffer->Buffer);
    if (FAILED(Result))
    {
        LogError("Failed to create buffer of size %d", Size);
    }
}

void GpuBufferFree(gpu_buffer *Buffer)
{
    SafeRelease(Buffer->Buffer);
}

void GpuBufferUploadData(gpu_buffer *Buffer, const void *Data)
{
    DxRenderContext.DeviceContext->UpdateSubresource(Buffer->Buffer, 0, nullptr, Data, 0, 0);
}

void GpuBufferBindVertex(gpu_buffer *Buffer)
{
    uint32_t Stride = Buffer->Stride;
    uint32_t Offset = 0;
    DxRenderContext.DeviceContext->IASetVertexBuffers(0, 1, &Buffer->Buffer, &Stride, &Offset);
}

void GpuBufferBindIndex(gpu_buffer *Buffer)
{
    DxRenderContext.DeviceContext->IASetIndexBuffer(Buffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
}

void GpuBufferBindConstant(gpu_buffer *Buffer, int Binding, gpu_resource_bind Bind)
{
    switch (Bind)
    {
        case gpu_resource_bind::Vertex:
        {
            DxRenderContext.DeviceContext->VSSetConstantBuffers(Binding, 1, &Buffer->Buffer);
        } break;
        case gpu_resource_bind::Pixel:
        {
            DxRenderContext.DeviceContext->PSSetConstantBuffers(Binding, 1, &Buffer->Buffer);
        } break;
        case gpu_resource_bind::Compute:
        {
            DxRenderContext.DeviceContext->CSSetConstantBuffers(Binding, 1, &Buffer->Buffer);
        } break;
    }
}
