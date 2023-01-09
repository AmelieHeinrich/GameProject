--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 01/01/2023 18:16
--

add_rules("mode.debug", "mode.release")

target("Game Project")
    set_rundir(".")
    add_files("sp/**.cpp")
    add_headerfiles("sp/**.hpp")
    add_includedirs("sp", "external", { public = true })
    add_syslinks("user32", "dsound", "gdi32", "kernel32", "d3d11", "d3dcompiler", "dxgi")
