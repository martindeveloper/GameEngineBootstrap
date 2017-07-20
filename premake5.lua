local Renderers = {
  OpenGL4 = {},
  DirectX11 = {},
  Vulkan = {}
}

local renderer_target = Renderers.OpenGL4;

function vulkan_get_path()
   local vulkan_version = "1.0.54.0"
   local vulkan_windows_root = "C:/VulkanSDK"

   return vulkan_windows_root .. "/" .. vulkan_version .. "/"
end

workspace "GameEngineBootstrap"
   configurations { "Debug", "Release" }
   location "ProjectFiles/"

   platforms { "Win32", "Win64" }

   if renderer_target == Renderers.Vulkan then
      links { "vulkan-1" }
      includedirs { vulkan_get_path() .. "Include" }
      defines { "RENDERER=RENDERER_VULKAN", "RENDERER_VULKAN=1" }
   end

   if renderer_target == Renderers.OpenGL4 then
      links { "opengl32", "glu32", "glew32s" }
      defines { "RENDERER=RENDERER_OPENGL4", "RENDERER_OPENGL4=1", "GLEW_STATIC" }
      includedirs { "External/glew/inc/" }
      files { "Source/Shaders/**.glsl" }
   end

   if renderer_target == Renderers.DirectX11 then
      defines { "RENDERER=RENDERER_DIRECTX11", "RENDERER_DIRECTX11=1" }
      files { "Source/Shaders/**.hlsl" }
   end

   filter "platforms:Win32"
      system "Windows"
      architecture "x32"
      
      if renderer_target == Renderers.Vulkan then
         libdirs { vulkan_get_path() .. "Lib32" }
      end
      
      if renderer_target == Renderers.OpenGL4 then
         libdirs { "External/glew/lib/Win32" }
      end

   filter "platforms:Win64"
      system "Windows"
      architecture "x64"

      if renderer_target == Renderers.Vulkan then
         libdirs { vulkan_get_path() .. "Lib" }
      end
      
      if renderer_target == Renderers.OpenGL4 then
         libdirs { "External/glew/lib/x64" }
      end

project "GameEngineBootstrap"
   kind "WindowedApp"
   language "C++"
   characterset "Unicode"
   flags{ "WinMain" }

   targetdir "Binaries/%{cfg.platform}-%{cfg.architecture}/%{cfg.buildcfg}"
   objdir "Intermediate/%{cfg.platform}-%{cfg.architecture}/%{cfg.buildcfg}"

   files { "Source/**.h", "Source/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"
