--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 01/01/2023 18:16
--

add_rules("mode.debug", "mode.release")

target("dr_libs")
    set_kind("static")
    add_files("external/dr_libs/*.cpp")
    add_headerfiles("external/dr_libs/*.h")
    add_includedirs("external/dr_libs/")

target("ImGui")
    set_kind("static")
    add_files("external/ImGui/**.cpp")
    add_headerfiles("external/ImGui/*.h")
    add_includedirs("external/ImGui/")
    add_syslinks("d3d11", "d3dcompiler", "dxgi")

target("Game Project")
    set_rundir(".")
    add_deps("ImGui", "dr_libs")
    add_files("sp/**.cpp")
    add_headerfiles("sp/**.hpp")
    add_includedirs("sp", "external", { public = true })
    add_syslinks("user32", "dsound", "gdi32", "kernel32", "d3d11", "d3dcompiler", "dxgi")
