local name = path.getbasename(os.getcwd())
local directory = os.getcwd()

-- Create directories
os.execute("mkdir " .. name)
os.execute("mkdir \"" .. directory .. "/" .. name .. "/includes\"")
os.execute("mkdir \"" .. directory .. "/" .. name .. "/src\"")
os.execute("mkdir " .. "vendor")
os.execute("mkdir " .. "scripts")
os.execute("mv main.cpp " .. name .. "/src/main.cpp")


workspace (name)
    configurations { "Example", "Dynamic Library", "Static Library Debug", "Static Library" }
    platforms { "Win32", "x64" }

project (name)
    location (name)
    kind "ConsoleApp"
    language "C++"

    filter "configurations:Example"
        defines { "EXAMPLE" }
        targetdir "bin/Example/%{cfg.buildcfg}%"
        staticruntime "Off"

    filter "configurations:Dynamic Library"
        kind "SharedLib"
        defines { "DYNAMIC_LIBRARY" }
        targetdir "bin/build/bin"
        staticruntime "Off"
        
    filter "configurations:Static Library Debug"
        kind "StaticLib"
        defines { "STATIC_LIBRARY_DEBUG" }
        symbols "On"
        targetextension "-d.lib"
        targetdir "bin/build/lib"
        staticruntime "On"
        
    filter "configurations:Static Library"
        kind "StaticLib"
        defines { "STATIC_LIBRARY" }
        staticruntime "On"
        targetextension ".lib"
        targetdir "bin/build/lib"

    filter "platforms:Win32"
        architecture "x86"

    filter "platforms:x64"
        architecture "x86_64"

    filter {} -- reset filter

    includedirs { name .. "/includes" }
    files { "**/.rc", "**/.h", "**/.cpp" }