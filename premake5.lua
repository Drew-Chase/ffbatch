-- Get the base name of the current working directory
local name = path.getbasename(os.getcwd())

-- Get the current working directory
local directory = os.getcwd()

-- Rename the "project" directory to the base name of the current working directory
os.execute("mv \"" .. directory .. "/project" .. "\" \"" .. directory .. "/" .. name .. "\"")

-- Create a new directory with the base name of the current working directory
os.execute("mkdir \"" .. name .. "\"")

-- Create a "includes" subdirectory under the new directory
os.execute("mkdir \"" .. directory .. "/" .. name .. "/includes\"")

-- Create a "src" subdirectory under the new directory
os.execute("mkdir \"" .. directory .. "/" .. name .. "/src\"")

-- Create a "vendor" directory in the current working directory
os.execute("mkdir vendor")

-- Define a workspace with the base name of the current working directory
workspace (name)

-- Define the configurations for the workspace
configurations { "Shared", "Shared (Debug)", "Static (Debug)", "Static" }

-- Define the platforms for the workspace
platforms { "x86", "x64" }

-- Define a project with the base name of the current working directory
project (name)

-- Set the location of the project
location (name)

-- Set the type of the project to a console application
kind "ConsoleApp"

-- Set the language of the project to C++
language "C++"

-- Set the C++ dialect of the project to C++20
cppdialect "C++20"

-- Set the C dialect of the project to C17
cdialect "C17"

-- Set the character set of the project to Unicode
characterset "Unicode"

-- Sets the opimization level of the project to "Full"
optimize "Full"

-- Define the settings for the "Dynamic Library" configuration
filter "configurations:Shared (Debug)"
    defines { "DYNAMIC_LIBRARY" }
    targetdir "bin/debug/shared"
    staticruntime "Off"
    symbols "On"
    objdir "bin/obj/debug/dll"

-- Define the settings for the "Dynamic Library" configuration
filter "configurations:Shared"
    defines { "DYNAMIC_LIBRARY" }
    targetdir "bin/release/shared"
    staticruntime "Off"
    symbols "Off"
    objdir "bin/obj/release/dll"

-- Define the settings for the "Static Library Debug" configuration
filter "configurations:Static (Debug)"
    defines { "STATIC_LIBRARY_DEBUG" }
    symbols "Off"
    targetdir "bin/debug/static"
    staticruntime "On"
    objdir "bin/obj/debug/lib-d"

-- Define the settings for the "Static Library" configuration
filter "configurations:Static"
    defines { "STATIC_LIBRARY" }
    staticruntime "On"
    targetdir "bin/release/static"
    objdir "bin/obj/release/lib"
    symbols "Off"

-- Define the architecture for the "Win32" platform
filter "platforms:x86"
    architecture "x86"
    targetname (name .. "-x86")

-- Define the architecture for the "x64" platform
filter "platforms:x64"
    architecture "x86_64"

-- Reset the filter
filter {}

-- Define the include directories for the project
includedirs { name .. "/includes", "include" }

-- Define the files to be included in the project
files { name .. "/**/*.rc", name .. "/**/*.h", name .. "/**/*.cpp", "**/src/**.h", "**/src/**.cpp" }