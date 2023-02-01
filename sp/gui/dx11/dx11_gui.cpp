/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 11/01/2023 09:50
 */

#include "gui/gui.hpp"

#include "gpu/dx11/dx11_context.hpp"
#include "systems/file_system.hpp"
#include "systems/log_system.hpp"
#include "windows/windows_data.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>

void GuiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();
    
    IO.DisplaySize = ImVec2(DxRenderContext.Width, DxRenderContext.Height);
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
    ImGui_ImplDX11_Init(DxRenderContext.Device, DxRenderContext.DeviceContext);
    ImGui_ImplWin32_Init(Win32.Window);
}

void GuiBeginFrame()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
}

void GuiEndFrame()
{   
    ImGuiIO& IO = ImGui::GetIO();
    IO.DisplaySize = ImVec2(DxRenderContext.Width, DxRenderContext.Height);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GuiExit()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
