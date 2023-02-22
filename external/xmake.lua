--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 22/02/2023 23:53
--

target("D3D12MA")
    set_kind("static")
    add_files("D3D12MA/*.cpp")
    add_headerfiles("D3D12MA/*.h")

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
    add_files("ImGui/**.cpp")
    add_headerfiles("ImGui/*.h")
    add_includedirs("ImGui/")
    add_syslinks("d3dcompiler", "dxgi", "d3d12")