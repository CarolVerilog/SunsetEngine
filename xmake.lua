add_rules("mode.debug", "mode.release")

target("sunset")
    set_kind("binary")
    set_languages("c++20")
    add_includedirs("inc")
    add_links("glfw","vulkan")
    add_files("src/**.cpp")

    if is_mode("debug") then
        add_defines("DEBUG")
    end