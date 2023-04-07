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

include "Judas-Engine/vendor/GLFW"
include "Judas-Engine/vendor/Glad"
include "Judas-Engine/vendor/imgui"

project "Judas-Engine"
    location "Judas-Engine"
    kind "SharedLib"
    staticruntime "off"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "jepch.h"
    pchsource "Judas-Engine/src/jepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
	"%{prj.name}/vendor/glm/glm/**.hpp",
	"%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
	"%{prj.name}/src",
	"%{prj.name}/vendor/spdlog/include",

        "%{IncludeDir.GLFW}",
	"%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
	"%{IncludeDir.glm}"
    }

    links 
    { 
	"GLFW",
	"Glad",
        "ImGui",
	"opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
            "JE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "JE_ENABLE_ASSERT"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
        }
    
    	filter "configurations:Debug"
		defines "JE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "JE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "JE_DIST"
		runtime "Release"
		optimize "On"

project "Game"
    location "Game"
    kind "ConsoleApp"
    staticruntime "off"
    language "C++"

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

        "%{IncludeDir.GLFW}",
	"%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
	"%{IncludeDir.glm}"

    }

    links
    {
        "Judas-Engine"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "JE_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "JE_DEBUG"
	runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "JE_RELEASE"
	runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "JE_DIST"
	runtime "Release"
        optimize "On"