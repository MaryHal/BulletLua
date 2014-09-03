solution "Pattern"
    configurations { "Debug", "Release" }
    platforms { "native", "x32", "x64" }

    project "test"
        language "C++"
        buildoptions { "-std=c++11" }

        includedirs { "../include", "../ext/sol/" }
        files { "src/**" }

        targetdir "bin"
        targetname "Test"

        ---------------------------------------
        -- Link static libraries and config
        libdirs { "../lib" }
        links { "sfml-graphics", "sfml-window", "sfml-system", "lua", "bulletlua" }

        configuration "linux"
            targetprefix "linux_"

        configuration "windows"
            targetprefix "windows_"

        configuration { "native or x64" }
            targetsuffix "64"

        configuration "x32"
            targetsuffix "32"

        ---------------------------------------
        -- Build rules
        configuration "Debug"
            kind "ConsoleApp"
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            kind "ConsoleApp"
            flags { "Optimize", "ExtraWarnings" }

