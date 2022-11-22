workspace "BMPFileViewer"
	configurations { "Debug", "Release" }
	platforms { "x64" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/GLFW"
include "vendor/Glad"
include "vendor/stb_image"
include "vendor/ImGui"

include "BMPFileViewer"