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
		"../vendor/imgui"
	}

	links
	{
		"GLFW",
		"GLAD",
		"stb_image",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "on"
		
	filter "configurations:Release"
		defines { "RELEASE" }
		optimize "on"