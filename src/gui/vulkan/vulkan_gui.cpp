/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 06/03/2023 10:07
 */

#include "gui/gui.hpp"

#include <gpu/vulkan/vulkan_context.hpp>
#include <gpu/vulkan/vulkan_command_buffer.hpp>
#include <systems/file_system.hpp>
#include <windows/windows_data.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_vulkan.h>
#include <ImGui/imgui_impl_win32.h>

void GuiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& IO = ImGui::GetIO();

    IO.DisplaySize = ImVec2(VK.Width, VK.Height);
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

    // TODO(amelie.h): Finish
    ImGui_ImplVulkan_InitInfo Info = {};
    Info.Device = VK.Device;
    Info.Instance = VK.Instance;
    Info.PhysicalDevice = VK.PhysicalDevice;
    Info.Queue = VK.GraphicsQueue;
    Info.QueueFamily = VK.GraphicsQueueFamily;

    ImGui_ImplWin32_EnableDpiAwareness();
    ImGui_ImplVulkan_Init(&Info, VK_NULL_HANDLE);
    ImGui_ImplWin32_Init(Win32.Window);
}

void GuiBeginFrame()
{
    ImGui::NewFrame();
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplWin32_NewFrame();
}

void GuiEndFrame(gpu_command_buffer *Buffer)
{
    vulkan_command_buffer *Private = (vulkan_command_buffer*)Buffer->Private;

    ImGuiIO& IO = ImGui::GetIO();
    IO.DisplaySize = ImVec2(VK.Width, VK.Height);

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Private->CommandBuffer);
    
    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void GuiExit()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
