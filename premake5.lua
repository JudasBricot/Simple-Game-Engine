workspace "Judas-Engine"
    architecture "x64"
    startproject "Game"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Judas-Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Judas-Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Judas-Engine/vendor/imgui"
IncludeDir["glm"] = "Judas-Engine/vendor/glm"
IncludeDir["stb_image"] = "Judas-Engine/vendor/stb_image"

include "Judas-Engine/vendor/GLFW"
include "Judas-Engine/vendor/Glad"
include "Judas-Engine/vendor/imgui"

project "Judas-Engine"
    location "Judas-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "jepch.h"
	pchsource "Judas-Engine/src/jepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
	"%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
	"%{prj.name}/vendor/glm/glm/**.hpp",
	"%{prj.name}/vendor/glm/glm/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
	"%{prj.name}/src",
	"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
	"%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
	"%{IncludeDir.glm}",
	"%{IncludeDir.stb_image}"
    }

    links 
    { 
	    "GLFW",
	    "Glad",
        "ImGui",
	    "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
            "JE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "JE_ENABLE_ASSERT",
        }
    
    filter "configurations:Debug"
		defines "JE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "JE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "JE_DIST"
		runtime "Release"
		optimize "on"

project "Game"
    location "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"


    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
	    "Judas-Engine/vendor/spdlog/include",
        "Judas-Engine/src",
	    "Judas-Engine/vendor",
	    "%{IncludeDir.glm}"

    }

    links
    {
        "Judas-Engine"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "JE_DEBUG"
	    runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "JE_RELEASE"
	    runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "JE_DIST"
	    runtime "Release"
        optimize "on"