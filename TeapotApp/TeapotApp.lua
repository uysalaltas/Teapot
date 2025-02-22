project "TeapotApp"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	debugdir "%{wks.location}"
	
	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Teapot/src",
		"%{wks.location}/Teapot/vendor/GLFW/include",
		"%{wks.location}/Teapot/vendor/GLAD/include",
		"%{wks.location}/Teapot/vendor/ImGui",
		"%{wks.location}/Teapot/vendor/ImGuizmo",
		"%{wks.location}/Teapot/vendor/glm",
		"%{wks.location}/Teapot/vendor/stb_image",
		"%{wks.location}/Teapot/vendor/assimp/include",
		"%{wks.location}/Teapot/vendor/pugi"
	}

	links
	{
		"Teapot"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TEA_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			("{COPYFILE} %{wks.location}Teapot/vendor/assimp/lib/assimp-vc142-mtd.dll %{wks.location}bin/" .. outputdir .. "/TeapotApp")
		}
	
	filter "system:linux"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		links 
		{
			"GLFW", "Xrandr", "Xi", "GLU", "GL", "X11", "dl", "pthread", "stdc++fs", "GLAD", "ImGui", "assimp"
		}

		defines
		{
			"TEA_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "TEA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TEA_RELEASE"
		optimize "On"