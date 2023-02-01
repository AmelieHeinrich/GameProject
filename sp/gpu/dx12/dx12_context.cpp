/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 01/02/2023 11:45
 */

#include "dx12_context.hpp"

#include "game_data.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

#include <string>

dx12_context DX12;

void GetHardwareAdapter(IDXGIFactory3 *Factory, IDXGIAdapter1 **RetAdapter, bool HighPerf)
{
    *RetAdapter = NULL;
    IDXGIAdapter1* Adapter = 0;
    IDXGIFactory6* Factory6;
    
    if (SUCCEEDED(Factory->QueryInterface(IID_PPV_ARGS(&Factory6)))) 
    {
        for (uint32_t AdapterIndex = 0; SUCCEEDED(Factory6->EnumAdapterByGpuPreference(AdapterIndex, HighPerf == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED, IID_PPV_ARGS(&Adapter))); ++AdapterIndex) 
        {
            DXGI_ADAPTER_DESC1 Desc;
            Adapter->GetDesc1(&Desc);

            if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice((IUnknown*)Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device*), NULL)))
                break;
        }
    }
    
    if (Adapter == NULL) {
        for (uint32_t AdapterIndex = 0; SUCCEEDED(Factory->EnumAdapters1(AdapterIndex, &Adapter)); ++AdapterIndex) 
        {
            DXGI_ADAPTER_DESC1 Desc;
            Adapter->GetDesc1(&Desc);

            if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                continue;

            if (SUCCEEDED(D3D12CreateDevice((IUnknown*)Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device*), NULL)))
                break;
        }
    }
    
    *RetAdapter = Adapter;
}

void GpuInit()
{
    bool Debug = EgcB32(EgcFile, "debug_rhi");
    
    if (Debug)
    {
        HRESULT Result = D3D12GetDebugInterface(IID_PPV_ARGS(&DX12.Debug));
        if (FAILED(Result))
            LogError("D3D12: Failed to get debug interface!");
        DX12.Debug->EnableDebugLayer();
    }

    HRESULT Result = CreateDXGIFactory(IID_PPV_ARGS(&DX12.Factory));
    if (FAILED(Result))
        LogError("D3D12: Failed to create DXGI factory!");
    GetHardwareAdapter(DX12.Factory, &DX12.Adapter, true);

    Result = D3D12CreateDevice(DX12.Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&DX12.Device));
    if (FAILED(Result))
        LogError("D3D12: Failed to create device!");
    
    if (Debug)
    {
        Result = DX12.Device->QueryInterface(IID_PPV_ARGS(&DX12.DebugDevice));
        if (FAILED(Result))
            LogError("D3D12: Failed to query debug device!");

        ID3D12InfoQueue* InfoQueue = 0;
        DX12.Device->QueryInterface(IID_PPV_ARGS(&InfoQueue));

        InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);

        D3D12_MESSAGE_SEVERITY SupressSeverities[] = {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        D3D12_MESSAGE_ID SupressIDs[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
        };

        D3D12_INFO_QUEUE_FILTER filter = {0};
        filter.DenyList.NumSeverities = ARRAYSIZE(SupressSeverities);
        filter.DenyList.pSeverityList = SupressSeverities;
        filter.DenyList.NumIDs = ARRAYSIZE(SupressIDs);
        filter.DenyList.pIDList = SupressIDs;

        InfoQueue->PushStorageFilter(&filter);
        InfoQueue->Release();
    }

    DXGI_ADAPTER_DESC Desc;
    DX12.Adapter->GetDesc(&Desc);
    std::wstring WideName = Desc.Description;
    std::string DeviceName = std::string(WideName.begin(), WideName.end());
    LogInfo("D3D12: Using GPU %s", DeviceName.c_str());
}

void GpuExit()
{
    bool Debug = EgcB32(EgcFile, "debug_rhi");

    SafeRelease(DX12.Device);
    SafeRelease(DX12.Factory);
    SafeRelease(DX12.Adapter);
    
    if (Debug)
    {
        DX12.DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_IGNORE_INTERNAL | D3D12_RLDO_DETAIL);
        SafeRelease(DX12.DebugDevice);
        SafeRelease(DX12.Debug);
    }
}
