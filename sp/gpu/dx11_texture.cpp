/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 12:44
 */

#include "dx11_texture.hpp"
#include "dx11_context.hpp"

#include "systems/log_system.hpp"

void GpuTextureInit(gpu_texture *Texture, int Width, int Height, DXGI_FORMAT Format, gpu_texture_bind Bind)
{
    Texture->Width = Width;
    Texture->Height = Height;
    Texture->Format = Format;

    D3D11_TEXTURE2D_DESC Desc = {};
    Desc.Width = Width;
    Desc.Height = Height;
    Desc.Format = Format;
    Desc.ArraySize = 1;
    Desc.BindFlags = (D3D11_BIND_FLAG)Bind;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.SampleDesc.Count = 1;
    Desc.MipLevels = 1;

    HRESULT Result = DxRenderContext.Device->CreateTexture2D(&Desc, NULL, &Texture->Texture);
    if (FAILED(Result))
    {
        LogError("Failed to create Texture2D!");
    }
}

void GpuTextureFree(gpu_texture *Texture)
{
    SafeRelease(Texture->Texture);
    SafeRelease(Texture->RTV);
    SafeRelease(Texture->DSV);
    SafeRelease(Texture->SRV);
    SafeRelease(Texture->UAV);
}

void GpuTextureInitRTV(gpu_texture *Texture)
{
    HRESULT Result = DxRenderContext.Device->CreateRenderTargetView(Texture->Texture, NULL, &Texture->RTV);
    if (FAILED(Result))
        LogError("Failed to create render target view!");
}

void GpuTextureInitDSV(gpu_texture *Texture, DXGI_FORMAT DepthFormat)
{
    HRESULT Result = DxRenderContext.Device->CreateDepthStencilView(Texture->Texture, NULL, &Texture->DSV);
    if (FAILED(Result))
        LogError("Failed to create depth stencil view!");
}

void GpuTextureInitSRV(gpu_texture *Texture)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC Desc = {};
    Desc.Format = (DXGI_FORMAT)Texture->Format;
    Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    Desc.Texture2D.MipLevels = 1;
    Desc.Texture2D.MostDetailedMip = 0;

    HRESULT Result = DxRenderContext.Device->CreateShaderResourceView(Texture->Texture, &Desc, &Texture->SRV);
    if (FAILED(Result))
        LogError("Failed to create shader resource view!");
}

void GpuTextureInitUAV(gpu_texture *Texture)
{
    D3D11_UNORDERED_ACCESS_VIEW_DESC Desc = {};
    Desc.Format = (DXGI_FORMAT)Texture->Format;
    Desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

    HRESULT Result = DxRenderContext.Device->CreateUnorderedAccessView(Texture->Texture, &Desc, &Texture->UAV);
    if (FAILED(Result))
        LogError("Failed to create unordered access view!");
}

void GpuTextureBindRTV(gpu_texture *Texture, gpu_texture *Depth, V4 ClearColor)
{
    ID3D11RenderTargetView* BindRTV = Texture->RTV;
    ID3D11DepthStencilView* BindDSV = nullptr;
    DxRenderContext.DeviceContext->ClearRenderTargetView(BindRTV, ClearColor.Elements);
    if (Depth) {
        BindDSV = Depth->DSV;
        DxRenderContext.DeviceContext->ClearDepthStencilView(BindDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    DxRenderContext.DeviceContext->OMSetRenderTargets(1, &BindRTV, BindDSV);
}

void GpuTextureBindSRV(gpu_texture *Texture, int Binding, gpu_resource_bind Bind)
{
    ID3D11ShaderResourceView* SRV[1] = { nullptr };
    SRV[0] = Texture->SRV;

    switch (Bind) {
        case gpu_resource_bind::Vertex: {
            DxRenderContext.DeviceContext->VSSetShaderResources(Binding, 1, SRV);
            return; 
        }
        case gpu_resource_bind::Pixel: {
            DxRenderContext.DeviceContext->PSSetShaderResources(Binding, 1, SRV);
            return; 
        }
        case gpu_resource_bind::Compute: {
            DxRenderContext.DeviceContext->CSSetShaderResources(Binding, 1, SRV);
            return;
        }
    }
}

void GpuTextureBindUAV(gpu_texture *Texture, int Binding)
{
    DxRenderContext.DeviceContext->CSSetUnorderedAccessViews(Binding, 1, &Texture->UAV, NULL);
}

void GpuResetRTV()
{
    ID3D11RenderTargetView* const RTV[1] = { NULL };
	ID3D11DepthStencilView* DSV = NULL;

    DxRenderContext.DeviceContext->OMSetRenderTargets(1, RTV, DSV);
}

void GpuResetSRV(int Binding, gpu_resource_bind Bind)
{
    ID3D11ShaderResourceView* SRV[1] = { nullptr };

    switch (Bind) {
        case gpu_resource_bind::Vertex: {
            DxRenderContext.DeviceContext->VSSetShaderResources(Binding, 1, SRV);
            return; 
        }
        case gpu_resource_bind::Pixel: {
            DxRenderContext.DeviceContext->PSSetShaderResources(Binding, 1, SRV);
            return; 
        }
        case gpu_resource_bind::Compute: {
            DxRenderContext.DeviceContext->CSSetShaderResources(Binding, 1, SRV);
            return;
        }
    }
}

void GpuResetUAV(int Binding)
{
    ID3D11UnorderedAccessView* const UAV[1] = { NULL };
    DxRenderContext.DeviceContext->CSSetUnorderedAccessViews(Binding, 1, UAV, NULL);
}
