solution "BulletLua"
    configurations { "Debug", "Release" }
    platforms { "native", "x32", "x64" }

    project "BulletLua"
        language "C++"
        kind "StaticLib"
        buildoptions { "-std=c++11" }

        includedirs { "include", "ext/sol" }
        files { "src/**" }

        targetdir "lib"
        targetname "bulletlua"

        ---------------------------------------
        -- Link static libraries and config
        libdirs { "ext/libbulletml/lib" }
        links { "lua" }

        ---------------------------------------
        -- Build rules
        configuration "Debug"
            targetsuffix "_d"
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            flags { "Optimize", "ExtraWarnings" }

    -- project "Test"
    --     language "C++"
    --     buildoptions { "-std=c++11" }

    --     includedirs { "./include/" }

    --     files { "test/**" }

    --     targetdir "bin"
    --     targetname "Test"

    --     ---------------------------------------
    --     -- Link static libraries and config
    --     libdirs ("lib")

    --     links { "SDL2" }

    --     configuration "linux"
    --         targetprefix "linux_"

    --     configuration "windows"
    --         targetprefix "windows_"

    --     configuration { "native or x64" }
    --         targetsuffix "64"

    --     configuration "x32"
    --         targetsuffix "32"

    --     ---------------------------------------
    --     -- Build rules
    --     configuration "Debug"
    --         links { "bulletlua_d" }

    --         kind "ConsoleApp"
    --         defines "DEBUG"
    --         flags { "Symbols", "ExtraWarnings" }

    --     configuration "Release"
    --         links { "bulletlua" }

    --         kind "ConsoleApp"
    --         flags { "Optimize", "ExtraWarnings" }


