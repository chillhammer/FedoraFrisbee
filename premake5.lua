workspace "FedoraFrisbee"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Vendor/Lib/GLFW"
	
project "FedoraFrisbee"
	location "Source"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Temp/Binary/" .. outputdir .. "/%{prj.name}")
	objdir ("Temp/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "fedpch.h"
	pchsource "Source/fedpch.cpp"

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs
	{
		"Source",
		"Vendor/Lib/spdlog/include",
		"Vendor/Lib/GLFW/include",
		"Vendor/Lib/GLEW/include",
	}

	links
	{
		"GLFW",
		"Vendor/Lib/GLFW/lib/Release/x64/glew32s.lib",
		"opengl32.lib"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "FED_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FED_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FED_DIST"
		optimize "On"