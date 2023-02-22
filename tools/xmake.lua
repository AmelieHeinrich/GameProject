--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 22/02/2023 23:53
--

target("egs_compiler")
    add_files("egs_compiler/*.cpp")
    set_rundir(".")

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
        add_syslinks("d3dcompiler")
    end
