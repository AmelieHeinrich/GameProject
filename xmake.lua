add_rules("mode.debug", "mode.release")

target("Game Project")
    add_files("sp/**.cpp")
    add_headerfiles("sp/**.hpp")
    add_includedirs("sp", { public = true })
    add_syslinks("user32")
