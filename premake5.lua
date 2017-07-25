function vulkan_get_path()
   local vulkan_version = "1.0.54.0"
   local vulkan_windows_root = "C:/VulkanSDK"

   return vulkan_windows_root .. "/" .. vulkan_version .. "/"
end

function glew_get_path()
   return "External/glew/"
end

function shaders_get_path()
   return "Source/Shaders/"
end

newoption {
   trigger = "renderer",
   value = "API",
   description = "Choose a particular 3D API for rendering",
   allowed = {
      { "opengl",    "OpenGL 4" },
      { "d3d11",  "Direct3D 11" },
      { "vulkan",  "Vulkan" }
   }
}

workspace "GameEngineBootstrap"
   configurations { "Debug", "Release" }
   location "ProjectFiles/"

   platforms { "Win32", "Win64" }

   filter "options:renderer=vulkan"
      links { "vulkan-1" }
      includedirs { vulkan_get_path() .. "Include" }
      defines { "RENDERER=RENDERER_VULKAN", "RENDERER_VULKAN=1" }

   filter "options:renderer=opengl"
      links { "opengl32", "glu32", "glew32s" }
      defines { "RENDERER=RENDERER_OPENGL4", "RENDERER_OPENGL4=1", "GLEW_STATIC" }
      includedirs { glew_get_path() .. "inc/" }
      files { shaders_get_path() .. "**.glsl" }

   filter "options:renderer=d3d11"
      links { "d3d11", "D3DCompiler" }
      defines { "RENDERER=RENDERER_DIRECTX11", "RENDERER_DIRECTX11=1" }
      files { shaders_get_path() .. "**.hlsl" }

   filter "platforms:Win32"
      system "Windows"
      architecture "x32"

      filter "options:renderer=vulkan"
         libdirs { vulkan_get_path() .. "Lib32" }

      filter "options:renderer=opengl"
         libdirs { glew_get_path() .. "lib/Win32" }

   filter "platforms:Win64"
      system "Windows"
      architecture "x64"

      filter "options:renderer=vulkan"
         libdirs { vulkan_get_path() .. "Lib" }

      filter "options:renderer=opengl"
         libdirs { glew_get_path() .. "lib/x64" }

project "GameEngineBootstrap"
   kind "WindowedApp"
   language "C++"
   characterset "Unicode"
   flags { "WinMain" }
   toolset "v141"

   targetdir "Binaries/%{cfg.platform}/%{cfg.buildcfg}"
   objdir "Intermediate/%{cfg.platform}/%{cfg.buildcfg}"
   debugdir "Binaries/%{cfg.platform}/%{cfg.buildcfg}"

   files { "Source/**.h", "Source/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"
