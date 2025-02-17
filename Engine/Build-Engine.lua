project "Engine"
   kind "SharedLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }
   IncludeDir = {}
   IncludeDir["GLFW"] = "RBVendor/glfw/include"
   IncludeDir["spdlogs"] ="RBVendor/spdlogs/include"

   includedirs
   {
      "Source",
      "%{IncludeDir.spdlogs}",
      "%{IncludeDir.GLFW}"
   }

   links
   {
        "GLFW",
        "opengl32.lib"
   }

   pchheader "pch.h"
   pchsource "Source/pch.cpp"

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
 
   postbuildcommands {
     "{COPYFILE} %{cfg.buildtarget.relpath} ../Binaries/" .. OutputDir .. "/App/%{cfg.buildtarget.name}"
}
   filter "system:windows"
       systemversion "latest"
       defines {"RUBBER_BUILD" }

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