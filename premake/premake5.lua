workspace "Portals"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solutiondir
IncludeDir = {}
IncludeDir["GLFW"] = "Dyson/vendor/GLFW/include"
IncludeDir["GLAD"] = "Dyson/vendor/GLAD/include"
IncludeDir["glm"] = "Dyson/vendor/glm"
IncludeDir["stb_image"] = "Dyson/vendor/stb_image"

include "Dyson/vendor/GLFW"
include "Dyson/vendor/GLAD"

project "Portals"
	location "../Portals"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "dypch.h"
	pchsource "Dyson/src/dypch.cpp"

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",		
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
--			"DY_PLATFORM_WINDOWS",
			"DY_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DY_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DY_DIST"
		runtime "Release"
		optimize "on"


