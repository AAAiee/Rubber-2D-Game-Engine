-- premake5.lua
workspace "Rubber Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" ,"/utf-8"}
      defines {"WINDOW_64_API"}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "GLFW"
   include "Engine/RBVendor/glfw/premake5.lua"
group "Engine"
	include "Engine/Build-Engine.lua"
group "App"
   include "App/Build-App.lua"