workspace "TeapotApp"
	architecture "x64"
	startproject "TeapotApp"
    configurations { "Debug", "Release"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Teapot/Teapot.lua"
include "TeapotApp/TeapotApp.lua"

include "Teapot/vendor/GLFW"