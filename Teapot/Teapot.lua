project "Teapot"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"vendor/GLFW/include"
	}

	links
	{
		"GLFW"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		links
		{
			"opengl32.lib"
		}

		defines
		{
			"TEA_PLATFORM_WINDOWS",
			"TEA_BUILD_DLL"
		}

		-- postbuildcommands
		-- {
		-- 	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TeapotApp")
		-- }

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		links 
		{
			"dl", "pthread" 
		}
		
		defines
		{
			"TEA_PLATFORM_LINUX",
		}

	filter "configurations:Debug"
		defines "TEA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TEA_RELEASE"
		runtime "Release"
		optimize "on"