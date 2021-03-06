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
	optimize "Size"

	targetdir ("Temp/Binary/" .. outputdir .. "/%{prj.name}")
	objdir ("Temp/Intermediates/" .. outputdir .. "/%{prj.name}")

	pchheader "FedPCH.h"
	pchsource "Source/PCH/FedPCH.cpp"

	files
	{
		"Source/**.h",
		"Source/**.cpp",
	}

	includedirs
	{
		"Source",
		"Source/FedGame",
		"Source/PCH",
		"Source/Utility",
		"Source/Vendor",
		"Vendor/Lib/spdlog/include",
		"Vendor/Lib/GLFW/include",
		"Vendor/Lib/GLEW/include",
		"Vendor/Lib/GLM",
		"Vendor/Lib/Assimp/include",
		"Vendor/Lib/freetype/include",
		"Vendor/Lib/irrKlang/include"
	}

	links
	{
		"GLFW",
		"Vendor/Lib/GLEW/lib/Release/x64/glew32s.lib",
		"Vendor/Lib/Assimp/lib/assimp.lib",
		"Vendor/Lib/freetype/lib/freetype.lib",
		"Vendor/Lib/irrKlang/lib/Winx64-visualStudio/irrKlang.lib",
		"opengl32.lib"
	}
	filter "system:windows"
		buildoptions { "-mwindows"}
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"FED_PLATFORM_WINDOWS",
			"GLEW_STATIC"
		}

	filter "configurations:Debug"
		defines 
		{
			"FED_DEBUG",
			"ENABLE_ASSERTS"
		}
		symbols "On"

	filter "configurations:Release"
		defines 
		{
			"FED_RELEASE",
			"ENABLE_ASSERTS"
		}
		optimize "On"

	filter "configurations:Dist"
		defines "FED_DIST"
		optimize "On"