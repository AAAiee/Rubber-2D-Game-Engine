project "Engine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   files { "Source/**.h", "Source/**.cpp" }
   IncludeDir = {}
   IncludeDir["GLFW"] = "RBVendor/glfw/include"
   IncludeDir["Glad"] = "RBVendor/glad/include"
   IncludeDir["spdlogs"] ="RBVendor/spdlogs/include"
   IncludeDir["imGui"] = "RBVendor/imGui"
   IncludeDir["glm"] = "RBVendor/glm"

   includedirs
   {
      "Source",
      "%{IncludeDir.spdlogs}",
      "%{IncludeDir.GLFW}",
      "%{IncludeDir.Glad}",
      "%{IncludeDir.imGui}",
      "%{IncludeDir.glm}"
   }

   links
   {
        "GLFW",
        "Glad",
        "imGui",
        "opengl32.lib"
   }

   pchheader "pch.h"
   pchsource "Source/pch.cpp"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
 
   defines {"_CRT_SECURE_NO_WARNINGS","RUBBER_BUILD", "GLFW_INCLUDE_NONE"}

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG", "RB_ENABLE_ASSERT" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"