--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 22/02/2023 23:53
--

target("D3D12MA")
    set_kind("static")

    if get_config("rhi") == "d3d12" then
        add_files("D3D12MA/*.cpp")
        add_headerfiles("D3D12MA/*.h")
    end

target("stb")
    set_kind("static")
    add_files("stb/stb.c")
    add_headerfiles("stb/*.h")
    add_includedirs("stb")

target("dr_libs")
    set_kind("static")
    add_files("dr_libs/*.cpp")
    add_headerfiles("dr_libs/*.h")
    add_includedirs("dr_libs/")

target("ImGui")
    set_kind("static")
    add_files("ImGui/*.cpp")
    add_headerfiles("ImGui/*.h")
    add_includedirs("ImGui/")
    
    if get_config("rhi") == "d3d12" then
        add_syslinks("d3dcompiler", "dxgi", "d3d12")
        remove_files("ImGui/imgui_impl_vulkan.cpp", "ImGui/imgui_impl_vulkan.h")
    elseif get_config("rhi") == "vulkan" then
        add_includedirs(os.getenv("VULKAN_SDK") .. "/Include")
        add_linkdirs(os.getenv("VULKAN_SDK") .. "/Lib")
        add_syslinks("vulkan-1")
        remove_files("ImGui/imgui_impl_dx12.cpp", "ImGui/imgui_impl_dx12.h")
    end
