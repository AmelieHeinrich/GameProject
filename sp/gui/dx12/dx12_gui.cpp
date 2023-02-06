/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 09:50
 */

#include "gui/gui.hpp"

#include "game_data.hpp"
#include "gpu/dx12/dx12_context.hpp"
#include "gpu/dx12/dx12_command_buffer.hpp"
#include "systems/file_system.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx12.h>
#include <ImGui/imgui_impl_win32.h>

struct dx12_gui
{
    uint32_t FontDescriptor;
};

dx12_gui GUI;

void GuiInit()
{
    GUI.FontDescriptor = Dx12DescriptorHeapAlloc(&DX12.CBVSRVUAVHeap);
    auto CPUHandle = Dx12DescriptorHeapCPU(&DX12.CBVSRVUAVHeap, GUI.FontDescriptor);
    auto GPUHandle = Dx12DescriptorHeapGPU(&DX12.CBVSRVUAVHeap, GUI.FontDescriptor);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();

    IO.DisplaySize = ImVec2(DX12.Width, DX12.Height);
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    if (FileBufferExists("assets/fonts/Roboto.ttf"))
        IO.Fonts->AddFontFromFileTTF("assets/fonts/Roboto.ttf", 16);

    ImGui::StyleColorsDark();

    ImGuiStyle& Style = ImGui::GetStyle();
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        Style.WindowRounding = 0.0f;
        Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_EnableDpiAwareness();
    ImGui_ImplDX12_Init(DX12.Device, EgcI32(EgcFile, "buffer_count"), DXGI_FORMAT_R8G8B8A8_UNORM, DX12.CBVSRVUAVHeap.Heap, CPUHandle, GPUHandle);
    ImGui_ImplWin32_Init(Win32.Window);
}

void GuiBeginFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GuiEndFrame(gpu_command_buffer *Buffer)
{   
    dx12_command_buffer *Private = (dx12_command_buffer*)Buffer->Private;
    Private->List->SetDescriptorHeaps(1, &DX12.CBVSRVUAVHeap.Heap);

    ImGuiIO& IO = ImGui::GetIO();
    IO.DisplaySize = ImVec2(DX12.Width, DX12.Height);

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Private->List);
    
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GuiExit()
{
    Dx12DescriptorHeapFreeSpace(&DX12.CBVSRVUAVHeap, GUI.FontDescriptor);
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
