-- premake5.lua
workspace "Rubber Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Game"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" ,"/utf-8"}
      defines {"WINDOW_64_API"}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
	include "Engine/Build-Engine.lua"
   include "Engine/RBVendor/glfw/Build-GLFW.lua"
   include "Engine/RBVendor/Glad/Build-Glad.lua"
   include "Engine/RBVendor/imGui/Build-imGui.lua"
group "GameExample"
   include "GameExample/Build-Game.lua"

group "Editor"
   include "Editor/Build-Editor.lua"
