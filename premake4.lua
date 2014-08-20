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
