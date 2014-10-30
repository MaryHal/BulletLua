solution "BulletLua"
    configurations { "Debug", "Release" }
    -- platforms { "native", "x32", "x64" }

    -- Main Library
    project "BulletLua"
        language "C++"
        kind "SharedLib"
        buildoptions { "-std=c++11" }

        includedirs { "include", "ext/sol" }
        files { "src/**" }

        targetdir "lib"
        targetname "bulletlua"

        -- Hacky workaround (combined with the linker options in the next project)
        -- so the Test application can conveniently find this shared library file.
        postbuildcommands { "cp lib/libbulletlua.so example/bin" }

        ---------------------------------------
        -- Link libraries
        libdirs { "ext/libbulletml/lib" }
        links { "lua" }

        ---------------------------------------
        -- Build rules
        configuration "Debug"
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            flags { "Optimize", "ExtraWarnings" }


    -- Test Application
    project "Test"
        language "C++"
        buildoptions { "-std=c++11" }

        includedirs { "include", "ext/sol/" }
        files { "example/src/**" }

        targetdir "example/bin"
        targetname "Test"

        ---------------------------------------
        -- Link libraries
        libdirs { "lib" }
        links { "sfml-graphics", "sfml-window", "sfml-system", "lua", "bulletlua" }

        -- Search for shared library files in the same directory as this executable.
        linkoptions { "-Wl,-rpath '-Wl,\$\$ORIGIN'" }

        ---------------------------------------
        -- Build rules
        configuration "Debug"
            kind "ConsoleApp"
            defines "DEBUG"
            flags { "Symbols", "ExtraWarnings" }

        configuration "Release"
            kind "ConsoleApp"
            flags { "Optimize", "ExtraWarnings" }
