-- include subprojects
includes("lib/commonlibsse")

-- set project constants
set_project("BakaWorldMapFOV")
set_version("3.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- require package dependencies
add_requires("glaze v7.0.0")

-- define targets
target("BakaWorldMapFOV")
    add_rules("commonlibsse.plugin", {
        name = "BakaWorldMapFOV",
        author = "shad0wshayd3"
    })

    -- bind package dependencies
    add_packages("glaze")

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")

    -- add install files
    add_installfiles("res/(**.json)")
