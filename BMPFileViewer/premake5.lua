project "BMPFileViewer"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	files { "src/**.h", "src/**.cpp" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{
		"src",
		"../vendor/glfw/include",
		"../vendor/glad/include",
		"../vendor/stb_image/include",
		"../vendor/imgui",
		"../vendor/tomlplusplus/include"
	}

	links
	{
		"GLFW",
		"GLAD",
		"stb_image",
		"ImGui",
		"toml++",
		"opengl32.lib"
	}

	defines
	{
		"TOML_HEADER_ONLY",
		"TOML_EXCEPTIONS=0"	
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"
		
	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "on"