/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 07/02/2023 14:30
 */

#pragma once

#include "gpu/gpu_pipeline.hpp"

#include <d3d12.h>
#include <string>
#include <unordered_map>

struct dx12_pipeline
{
    ID3D12RootSignature *Signature;
    ID3D12PipelineState *Pipeline;
    
    std::unordered_map<std::string, int> Bindings;
};
