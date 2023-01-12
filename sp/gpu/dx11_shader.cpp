/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 09:40
 */

#include "dx11_shader.hpp"
#include "dx11_context.hpp"

#include "systems/log_system.hpp"
#include "systems/file_system.hpp"

std::string GetEntryPointFromProfile(const std::string& Profile)
{
    if (Profile == "vs_5_0")
        return "VSMain";
    if (Profile == "ps_5_0")
        return "PSMain";
    if (Profile == "cs_5_0")
        return "CSMain";
}

ID3DBlob* CompileBlob(const std::string& Source, const char *Profile)
{
    ID3DBlob* ShaderBlob;
    ID3DBlob* ErrorBlob;
    D3DCompile(Source.c_str(), 
               Source.length(), 
               nullptr, 
               nullptr, 
               D3D_COMPILE_STANDARD_FILE_INCLUDE, 
               GetEntryPointFromProfile(Profile).c_str(), 
               Profile, 
               0, 
               0, 
               &ShaderBlob, 
               &ErrorBlob);
    if (ErrorBlob)
    {
        LogError("Shader Error (Profile: %s) : %s", Profile, (char*)ErrorBlob->GetBufferPointer());
        SafeRelease(ErrorBlob);
        exit(-1);
    }
    return ShaderBlob;
}

void GpuShaderInit(gpu_shader *Shader, const char *V,
                                       const char *P,
                                       const char *C)
{
    ID3DBlob* VS = nullptr;
    ID3DBlob* PS = nullptr;
    ID3DBlob* CS = nullptr;

    if (V)
    {
        VS = CompileBlob(FileRead(V), "vs_5_0");

        HRESULT Result = DxRenderContext.Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &Shader->VS);
        if (FAILED(Result))
            LogError("Failed to create vertex shader!");
    }
    if (P)
    {
        PS = CompileBlob(FileRead(P), "ps_5_0");

        HRESULT Result = DxRenderContext.Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &Shader->PS);
        if (FAILED(Result))
            LogError("Failed to create pixel shader!");
    }
    if (C)
    {
        CS = CompileBlob(FileRead(C), "cs_5_0");

        HRESULT Result = DxRenderContext.Device->CreateComputeShader(CS->GetBufferPointer(), CS->GetBufferSize(), nullptr, &Shader->CS);
        if (FAILED(Result))
            LogError("Failed to create compute shader!");
    }

    if (VS)
    {
        ID3D11ShaderReflection* Reflect = nullptr;
        HRESULT Result = D3DReflect(VS->GetBufferPointer(), VS->GetBufferSize(), IID_PPV_ARGS(&Reflect));
        if (FAILED(Result))
        {
            LogError("Failed to reflect vertex shader!");
        }

        D3D11_SHADER_DESC ShaderDesc;
        Reflect->GetDesc(&ShaderDesc);
        
        std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc;
        for (uint32_t ParameterIndex = 0; ParameterIndex < ShaderDesc.InputParameters; ParameterIndex++)
        {
            D3D11_SIGNATURE_PARAMETER_DESC ParamDesc;
            Reflect->GetInputParameterDesc(ParameterIndex, &ParamDesc);

            D3D11_INPUT_ELEMENT_DESC ElementDesc;
            ElementDesc.SemanticName = ParamDesc.SemanticName;
            ElementDesc.SemanticIndex = ParamDesc.SemanticIndex;
            ElementDesc.InputSlot = 0;
            ElementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            ElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            ElementDesc.InstanceDataStepRate = 0;   

            if (ParamDesc.Mask == 1)
            {
                if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32_UINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32_SINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
            }
            else if (ParamDesc.Mask <= 3)
            {
                if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
            }
            else if (ParamDesc.Mask <= 7)
            {
                if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            }
            else if (ParamDesc.Mask <= 15)
            {
                if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
                else if (ParamDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) ElementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            }

            InputLayoutDesc.push_back(ElementDesc);
        }

        if (FAILED(DxRenderContext.Device->CreateInputLayout(&InputLayoutDesc[0], (UINT)InputLayoutDesc.size(), VS->GetBufferPointer(), VS->GetBufferSize(), &Shader->Layout))) {
            LogError("Failed to create input layout!");
        }

        SafeRelease(Reflect);
    }

    SafeRelease(CS);
    SafeRelease(PS);
    SafeRelease(VS);
}

void GpuShaderFree(gpu_shader *Shader)
{
    SafeRelease(Shader->Layout);
    SafeRelease(Shader->CS);
    SafeRelease(Shader->PS);
    SafeRelease(Shader->VS);
}

void GpuShaderBind(gpu_shader *Shader)
{
    if (Shader->VS) DxRenderContext.DeviceContext->VSSetShader(Shader->VS, nullptr, 0);
    if (Shader->PS) DxRenderContext.DeviceContext->PSSetShader(Shader->PS, nullptr, 0);
    if (Shader->CS) DxRenderContext.DeviceContext->CSSetShader(Shader->CS, nullptr, 0);
    if (Shader->Layout) DxRenderContext.DeviceContext->IASetInputLayout(Shader->Layout);
}
