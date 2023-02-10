/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 07/02/2023 14:32
 */

#include "dx12_pipeline.hpp"

#include "dx12_context.hpp"
#include "dx12_shader.hpp"
#include "dx12_image.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

#include <d3dcompiler.h>
#include <algorithm>
#include <array>

D3D12_FILL_MODE GetDx12FillMode(fill_mode Mode)
{
    switch (Mode)
    {
        case fill_mode::Line:
            return D3D12_FILL_MODE_WIREFRAME;
        case fill_mode::Solid:
            return D3D12_FILL_MODE_SOLID;
    }
}

D3D12_CULL_MODE GetDx12CullMode(cull_mode Mode)
{
    switch (Mode)
    {
        case cull_mode::Back:
            return D3D12_CULL_MODE_BACK;
        case cull_mode::Front:
            return D3D12_CULL_MODE_FRONT;
        case cull_mode::None:
            return D3D12_CULL_MODE_NONE;
    }
}

D3D12_COMPARISON_FUNC GetDx12ComparisonFunc(depth_func Func)
{
    switch (Func)
    {
        case depth_func::Greater:
            return D3D12_COMPARISON_FUNC_GREATER;
        case depth_func::Less:
            return D3D12_COMPARISON_FUNC_LESS;
        case depth_func::LessEqual:
            return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case depth_func::None:
            return D3D12_COMPARISON_FUNC_NEVER;
    }
}

bool CompareShaderInput(const D3D12_SHADER_INPUT_BIND_DESC& A, const D3D12_SHADER_INPUT_BIND_DESC& B)
{
    return A.BindPoint < B.BindPoint;
}

void GpuPipelineCreateGraphics(gpu_pipeline *Pipeline)
{
    Pipeline->Private = new dx12_pipeline;
    Pipeline->Info.Type = gpu_pipeline_type::Graphics;

    dx12_pipeline *PipelinePrivate = (dx12_pipeline*)Pipeline->Private;
    dx12_shader *ShaderPrivate = (dx12_shader*)Pipeline->Info.Shader->Private;

    ID3D12ShaderReflection* VertexReflection = nullptr;
    D3D12_SHADER_DESC VertexDesc;
    
    std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
    std::vector<std::string> InputElementSemanticNames;

    std::array<D3D12_ROOT_PARAMETER, 64> Parameters;
    int ParameterCount = 0;

    std::array<D3D12_DESCRIPTOR_RANGE, 64> Ranges;
    int RangeCount = 0;

    std::array<D3D12_SHADER_INPUT_BIND_DESC, 64> ShaderBinds;
    int BindCount = 0;

    ID3D12ShaderReflection* PixelReflection = nullptr;
    D3D12_SHADER_DESC PixelDesc;

    HRESULT Result = D3DReflect(ShaderPrivate->VertexBlob->GetBufferPointer(), ShaderPrivate->VertexBlob->GetBufferSize(), IID_PPV_ARGS(&VertexReflection));
    if (FAILED(Result))
        LogError("D3D12: Failed to reflect vertex shader!");
    VertexReflection->GetDesc(&VertexDesc);

    Result = D3DReflect(ShaderPrivate->PixelBlob->GetBufferPointer(), ShaderPrivate->PixelBlob->GetBufferSize(), IID_PPV_ARGS(&PixelReflection));
    if (FAILED(Result))
        LogError("D3D12: Failed to reflect pixel shader!");
    PixelReflection->GetDesc(&PixelDesc);

    for (int BoundResourceIndex = 0; BoundResourceIndex < VertexDesc.BoundResources; BoundResourceIndex++)
    {
        D3D12_SHADER_INPUT_BIND_DESC ShaderInputBindDesc = {};
        VertexReflection->GetResourceBindingDesc(BoundResourceIndex, &ShaderInputBindDesc);
        ShaderBinds[BindCount] = ShaderInputBindDesc;
        BindCount++;
    }

    for (int BoundResourceIndex = 0; BoundResourceIndex < PixelDesc.BoundResources; BoundResourceIndex++)
    {
        D3D12_SHADER_INPUT_BIND_DESC ShaderInputBindDesc = {};
        PixelReflection->GetResourceBindingDesc(BoundResourceIndex, &ShaderInputBindDesc);
        ShaderBinds[BindCount] = ShaderInputBindDesc;
        BindCount++;
    }

    std::sort(ShaderBinds.begin(), ShaderBinds.begin() + BindCount, CompareShaderInput);

    for (int ShaderBindIndex = 0; ShaderBindIndex < BindCount; ShaderBindIndex++)
    {
        auto ShaderInputBindDesc = ShaderBinds[ShaderBindIndex];

        D3D12_ROOT_PARAMETER RootParameter = {};
        RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

        D3D12_DESCRIPTOR_RANGE Range = {};
        Range.NumDescriptors = 1;
        Range.BaseShaderRegister = ShaderInputBindDesc.BindPoint;

        switch (ShaderInputBindDesc.Type)
        {
            case D3D_SIT_SAMPLER:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                break;
            case D3D_SIT_TEXTURE:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                break;
            case D3D_SIT_UAV_RWTYPED:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                break;
            case D3D_SIT_CBUFFER:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                break;
            default:
                LogWarn("D3D12: Unsupported shader resource!");
                continue;
        }
        
        Ranges[RangeCount] = Range;

        RootParameter.DescriptorTable.NumDescriptorRanges = 1;
        RootParameter.DescriptorTable.pDescriptorRanges = &Ranges[RangeCount];
        RootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        Parameters[ParameterCount] = RootParameter;

        ParameterCount++;
        RangeCount++;
    }

    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = ParameterCount;
    RootSignatureDesc.pParameters = Parameters.data();
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob *RootSignatureBlob;
    ID3DBlob *ErrorBlob;
    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &RootSignatureBlob, &ErrorBlob);
    if (ErrorBlob)
        LogError("D3D12: Failed to serialize root signature! %s", ErrorBlob->GetBufferPointer());
    Result = DX12.Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelinePrivate->Signature));
    if (FAILED(Result))
        LogError("D3D12: Failed to create root signature!");
    RootSignatureBlob->Release();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC Desc = {};
    Desc.VS.pShaderBytecode = ShaderPrivate->VertexBlob->GetBufferPointer();
    Desc.VS.BytecodeLength = ShaderPrivate->VertexBlob->GetBufferSize();
    Desc.PS.pShaderBytecode = ShaderPrivate->PixelBlob->GetBufferPointer();
    Desc.PS.BytecodeLength = ShaderPrivate->PixelBlob->GetBufferSize();
    for (int RTVIndex = 0; RTVIndex < Pipeline->Info.Formats.size(); RTVIndex++)
    {
        Desc.BlendState.RenderTarget[RTVIndex].SrcBlend = D3D12_BLEND_ONE;
        Desc.BlendState.RenderTarget[RTVIndex].DestBlend = D3D12_BLEND_ZERO;
        Desc.BlendState.RenderTarget[RTVIndex].BlendOp = D3D12_BLEND_OP_ADD;
        Desc.BlendState.RenderTarget[RTVIndex].SrcBlendAlpha = D3D12_BLEND_ONE;
        Desc.BlendState.RenderTarget[RTVIndex].DestBlendAlpha = D3D12_BLEND_ZERO;
        Desc.BlendState.RenderTarget[RTVIndex].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        Desc.BlendState.RenderTarget[RTVIndex].LogicOp = D3D12_LOGIC_OP_NOOP;
        Desc.BlendState.RenderTarget[RTVIndex].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        Desc.RTVFormats[RTVIndex] = GetDXGIFormat(Pipeline->Info.Formats[RTVIndex]);
        Desc.NumRenderTargets = Pipeline->Info.Formats.size();
    }
    Desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    Desc.RasterizerState.FillMode = GetDx12FillMode(Pipeline->Info.FillMode);
    Desc.RasterizerState.CullMode = GetDx12CullMode(Pipeline->Info.CullMode);
    Desc.RasterizerState.DepthClipEnable = true;
    Desc.RasterizerState.FrontCounterClockwise = true;
    Desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    if (Pipeline->Info.HasDepth)
    {
        Desc.DepthStencilState.DepthEnable = true;
        Desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        Desc.DepthStencilState.DepthFunc = GetDx12ComparisonFunc(Pipeline->Info.DepthFunc);
        Desc.DSVFormat = GetDXGIFormat(Pipeline->Info.DepthFormat);
    }
    Desc.SampleDesc.Count = 1;

    // Reflect input layout
    InputElementSemanticNames.reserve(VertexDesc.InputParameters);
    InputElementDescs.reserve(VertexDesc.InputParameters);

    for (int ParameterIndex = 0; ParameterIndex < VertexDesc.InputParameters; ParameterIndex++)
    {
        D3D12_SIGNATURE_PARAMETER_DESC ParameterDesc = {};
        VertexReflection->GetInputParameterDesc(ParameterIndex, &ParameterDesc);
        
        InputElementSemanticNames.push_back(ParameterDesc.SemanticName);

        D3D12_INPUT_ELEMENT_DESC InputElement = {};
        InputElement.SemanticName = InputElementSemanticNames.back().c_str();
        InputElement.SemanticIndex = ParameterDesc.SemanticIndex;
        InputElement.InputSlot = 0;
        InputElement.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
        InputElement.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        InputElement.InstanceDataStepRate = 0;

        if (ParameterDesc.Mask == 1)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 3)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 7)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (ParameterDesc.Mask <= 15)
        {
            if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (ParameterDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) InputElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        InputElementDescs.push_back(InputElement);
    }
    Desc.InputLayout.pInputElementDescs = InputElementDescs.data();
    Desc.InputLayout.NumElements = static_cast<uint32_t>(InputElementDescs.size());
    Desc.pRootSignature = PipelinePrivate->Signature;

    Result = DX12.Device->CreateGraphicsPipelineState(&Desc, IID_PPV_ARGS(&PipelinePrivate->Pipeline));
    if (FAILED(Result)) 
        LogError("D3D12: Failed to create graphics pipeline state!");

    SafeRelease(PixelReflection);
    SafeRelease(VertexReflection);
}

void GpuPipelineCreateCompute(gpu_pipeline *Pipeline, gpu_pipeline_create_info *Info)
{
    Pipeline->Private = new dx12_pipeline;
    Pipeline->Info.Type = gpu_pipeline_type::Compute;

    dx12_pipeline *PipelinePrivate = (dx12_pipeline*)Pipeline->Private;
    dx12_shader *ShaderPrivate = (dx12_shader*)Pipeline->Info.Shader->Private;

    ID3D12ShaderReflection* ComputeReflection = nullptr;
    D3D12_SHADER_DESC ComputeDesc;

    std::vector<D3D12_ROOT_PARAMETER> Parameters;
    int ParameterCount = 0;

    std::vector<D3D12_SHADER_INPUT_BIND_DESC> ShaderBinds;
    int BindCount = 0;
    
    std::vector<D3D12_DESCRIPTOR_RANGE> Ranges;
    int RangeCount = 0;

    HRESULT Result = D3DReflect(ShaderPrivate->ComputeBlob->GetBufferPointer(), ShaderPrivate->ComputeBlob->GetBufferSize(), IID_PPV_ARGS(&ComputeReflection));
    if (FAILED(Result))
        LogError("D3D12: Failed to reflect vertex shader!");

    for (int BoundResourceIndex = 0; BoundResourceIndex < ComputeDesc.BoundResources; BoundResourceIndex++)
    {
        D3D12_SHADER_INPUT_BIND_DESC ShaderInputBindDesc = {};
        ComputeReflection->GetResourceBindingDesc(BoundResourceIndex, &ShaderInputBindDesc);
        ShaderBinds[BindCount] = ShaderInputBindDesc;
        BindCount++;
    }

    std::sort(ShaderBinds.begin(), ShaderBinds.begin() + BindCount, CompareShaderInput);

    for (int ShaderBindIndex = 0; ShaderBindIndex < BindCount; ShaderBindIndex++)
    {
        auto ShaderInputBindDesc = ShaderBinds[ShaderBindIndex];

        D3D12_ROOT_PARAMETER RootParameter = {};
        RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

        D3D12_DESCRIPTOR_RANGE Range = {};
        Range.NumDescriptors = 1;
        Range.BaseShaderRegister = ShaderInputBindDesc.BindPoint;

        switch (ShaderInputBindDesc.Type)
        {
            case D3D_SIT_SAMPLER:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                break;
            case D3D_SIT_TEXTURE:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                break;
            case D3D_SIT_UAV_RWTYPED:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                break;
            case D3D_SIT_CBUFFER:
                Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                break;
            default:
                LogWarn("D3D12: Unsupported shader resource!");
                continue;
        }
        
        Ranges[RangeCount] = Range;

        RootParameter.DescriptorTable.NumDescriptorRanges = 1;
        RootParameter.DescriptorTable.pDescriptorRanges = &Ranges[RangeCount];
        RootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        Parameters[ParameterCount] = RootParameter;

        ParameterCount++;
        RangeCount++;
    }

    D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
    RootSignatureDesc.NumParameters = Parameters.size();
    RootSignatureDesc.pParameters = Parameters.data();
    RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    ID3DBlob *RootSignatureBlob;
    ID3DBlob *ErrorBlob;
    D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &RootSignatureBlob, &ErrorBlob);
    if (ErrorBlob)
        LogError("D3D12: Failed to serialize root signature! %s", ErrorBlob->GetBufferPointer());
    Result = DX12.Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelinePrivate->Signature));
    if (FAILED(Result))
        LogError("D3D12: Failed to create root signature!");
    RootSignatureBlob->Release();

    D3D12_COMPUTE_PIPELINE_STATE_DESC Desc = {};
    Desc.CS.pShaderBytecode = ShaderPrivate->ComputeBlob->GetBufferPointer();
    Desc.CS.BytecodeLength = ShaderPrivate->ComputeBlob->GetBufferSize();
    Desc.pRootSignature = PipelinePrivate->Signature;

    Result = DX12.Device->CreateComputePipelineState(&Desc, IID_PPV_ARGS(&PipelinePrivate->Pipeline));
    if (FAILED(Result)) 
        LogError("D3D12: Failed to create compute pipeline state!");
}

void GpuPipelineFree(gpu_pipeline *Pipeline)
{
    dx12_pipeline *PipelinePrivate = (dx12_pipeline*)Pipeline->Private;
    SafeRelease(PipelinePrivate->Signature);
    SafeRelease(PipelinePrivate->Pipeline);
    delete Pipeline->Private;
}

int GpuPipelineGetDescriptor(gpu_pipeline *Pipeline, const std::string& Name)
{
    dx12_pipeline *PipelinePrivate = (dx12_pipeline*)Pipeline->Private;
    return PipelinePrivate->Bindings[Name];
}
