/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 09/02/2023 10:15
 */

#pragma once

#include <cstdint>

struct gpu_pipeline_statistics
{
    uint64_t IAVertices;
    uint64_t IAPrimitives;
    uint64_t VSInvocations;
    uint64_t GSInvocations;
    uint64_t GSPrimitives;
    uint64_t CInvocations;
    uint64_t CPrimitives;
    uint64_t PSInvocations;
    uint64_t HSInvocations;
    uint64_t DSInvocations;
    uint64_t CSInvocations;
};

struct gpu_pipeline_profiler
{
    gpu_pipeline_statistics Stats;
    void *Private;
};

void GpuPipelineProfilerInit(gpu_pipeline_profiler *Profiler);
void GpuPipelineProfilerFree(gpu_pipeline_profiler *Profiler);
