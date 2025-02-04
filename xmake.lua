-- xmake.lua

add_rules("mode.debug", "mode.release")

includes("ext")

target("MafiaShootout")
    set_rundir(".")
    add_files("src/**.cpp")
    set_languages("c++17")
    add_includedirs("SFML/include", "ext")
    add_linkdirs("SFML/lib")
    add_deps("ImGui")

    add_syslinks("user32", "sfml-graphics", "sfml-system", "sfml-window", "sfml-audio")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end

    before_link(function (target)
        os.cp("assets", "$(buildir)/$(plat)/$(arch)/$(mode)/")
        os.cp("SFML/bin/*", "$(buildir)/$(plat)/$(arch)/$(mode)/")
    end)
