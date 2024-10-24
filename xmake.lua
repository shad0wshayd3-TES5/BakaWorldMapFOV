-- set xmake version
set_xmakever("2.9.4")

-- include local folders
includes("extern/commonlibsse")

-- add custom package repository
add_repositories("xre https://github.com/shad0wshayd3/custom-xrepo")

-- set project
set_project("BakaWorldMapFOV")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_arch("x64")
set_languages("c++23")
set_optimize("faster")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- enable lto
set_policy("build.optimization.lto", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- compile for ae
set_config("skyrim_ae", true)

-- require package dependencies
add_requires("figcone", {configs = {use_ini = true}})

-- setup targets
target("BakaWorldMapFOV")
    -- bind local dependencies
    add_deps("commonlibsse")

    -- bind package dependencies
    add_packages("figcone")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "BakaWorldMapFOV",
        author = "shad0wshayd3"
    })

    -- add source files
    add_files("src/*.cpp")
    add_headerfiles("src/*.h")
    add_includedirs("src")
    set_pcxxheader("src/PCH.h")

    -- add extra files
    add_extrafiles(".clang-format")
