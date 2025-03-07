project "Glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("../../../Binaries/" .. OutputDir .. "/%{prj.name}")
	objdir ("../../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter { "system:windows"}	
		systemversion "latest"




