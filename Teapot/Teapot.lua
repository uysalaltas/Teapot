project "Teapot"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"ImGuiBuild.cpp",
		"vendor/ImGuizmo/*.cpp",
		"vendor/ImGuizmo/*.h"
	}

	includedirs
	{
		"src",
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/ImGui",
		"vendor/glm",
		"vendor/stb_image",
		"vendor/assimp/include",
		"vendor/ImGuizmo"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		libdirs
		{
			"vendor/assimp/lib"
		}

		links
		{
			"opengl32.lib",
			"assimp-vc142-mtd.lib"
		}

		defines
		{
			"TEA_PLATFORM_WINDOWS",
			"TEA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "system:linux"
		cppdialect "C++20"
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