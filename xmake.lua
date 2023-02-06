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
    add_syslinks("d3dcompiler", "dxgi", "d3d12")

target("Game Project")
    set_languages("c11", "c++17")
    set_rundir(".")
    add_deps("ImGui", "dr_libs")
    add_files("sp/*.cpp", "sp/cameras/*.cpp", "sp/gui/*.cpp", "sp/renderer/*.cpp", "sp/scene/*.cpp", "sp/systems/*.cpp")
    add_headerfiles("sp/**.hpp")
    add_includedirs("sp", "external", { public = true })
    add_linkdirs("bin/")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    if is_plat("windows") then
        add_syslinks("user32", "dsound", "gdi32", "kernel32", "d3d12", "d3dcompiler", "dxgi", "assimp-vc143-mtd")
        add_files("sp/apu/dsound/*.cpp", "sp/gpu/dx12/*.cpp", "sp/gui/dx12/*.cpp", "sp/systems/windows/*.cpp", "sp/windows/*.cpp")
        add_files("sp/main_win32.cpp")
    end
    -- TODO(amelie.h): MacOS
    -- TODO(amelie.h): Linux
    -- TODO(amelie.h): Nintendo Switch? :flushed:
