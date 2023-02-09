/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 10:17
 */

#include "dx12_pipeline_profiler.hpp"

#include "dx12_context.hpp"

#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

void GpuPipelineProfilerInit(gpu_pipeline_profiler *Profiler)
{
    Profiler->Private = new dx12_pipeline_profiler;
    dx12_pipeline_profiler *Private = (dx12_pipeline_profiler*)Profiler->Private;

    D3D12_QUERY_HEAP_DESC Desc = {};
    Desc.Count = 1;
    Desc.Type = D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS;

    HRESULT Result = DX12.Device->CreateQueryHeap(&Desc, IID_PPV_ARGS(&Private->Heap));
    if (FAILED(Result))
        LogError("D3D12: Failed to create pipeline profiler!");

    GpuBufferInit(&Private->Buffer, sizeof(D3D12_QUERY_DATA_PIPELINE_STATISTICS), 0, gpu_buffer_type::Vertex);
}

void GpuPipelineProfilerFree(gpu_pipeline_profiler *Profiler)
{
    dx12_pipeline_profiler *Private = (dx12_pipeline_profiler*)Profiler->Private;
    GpuBufferFree(&Private->Buffer);
    SafeRelease(Private->Heap);
    delete Private;
}
