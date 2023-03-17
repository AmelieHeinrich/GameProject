/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 17/03/2023 11:42
 */

#include "gpu/vulkan/vulkan_context.hpp"

#include "windows/windows_data.hpp"

VkResult CreateSurface(VkInstance Instance, VkSurfaceKHR *Surface)
{
    VkWin32SurfaceCreateInfoKHR SurfaceInfo = {};
    SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    SurfaceInfo.hinstance = Win32.Instance;
    SurfaceInfo.hwnd = Win32.Window;
    
    return vkCreateWin32SurfaceKHR(Instance, &SurfaceInfo, nullptr, Surface);
}
