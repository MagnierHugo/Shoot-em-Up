-- dependencies xmake.lua

target("ImGui")
    set_kind("static")
    add_files("imgui/**.cpp")
    set_languages("c++17")
    add_includedirs("../SFML/include", "imgui")
    add_linkdirs("../SFML/lib")

    add_syslinks("user32", "opengl32", "sfml-graphics", "sfml-system", "sfml-window")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
