-- Premake4 script to generate project makefiles.
-- This is not maintained.

solution "BulletLua"
    configurations { "Debug", "Release" }
    -- platforms { "native", "x32", "x64" }

    -- Main Library
    project "BulletLua"
        language "C++"
        kind "StaticLib"
        buildoptions { "-std=c++11" }

        includedirs { "include", "ext/sol", "ext/Catch/include" }
        files { "src/**" }

        targetdir "lib"
        targetname "bulletlua"

        ---------------------------------------
        -- Link libraries
        libdirs {}
        links { "lua" }

        ---------------------------------------
        -- Build rules
        configuration "Debug"
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            flags { "Optimize", "ExtraWarnings" }
