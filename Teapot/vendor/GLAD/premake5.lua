project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/glad/khrplatform.h",
        "src/glad.c",
    }

    includedirs
    {
        "include"
    }

    filter "system:linux"
        pic "On"
        systemversion "latest"
        staticruntime "On"

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
