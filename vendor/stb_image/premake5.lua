project "stb_image"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/stb_image.h",
		"include/stb_image_write.h",
        "src/stb_image.cpp"
    }

    includedirs
    {
        "include"
    }

    defines 
	{ 
		"_CRT_SECURE_NO_WARNINGS"
	}
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
