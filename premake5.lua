function vulkan_get_path()
   local vulkan_version = "1.0.68.0"
   local vulkan_windows_root = "C:/VulkanSDK"

   return vulkan_windows_root .. "/" .. vulkan_version .. "/"
end

function d3d11_compile_shader_command(shader)
   command = 'fxc.exe '
   command = command .. '"' .. shader.source_path .. '" '
   command = command .. '/nologo /T ' .. shader.target_profile .. ' /Fo "%{cfg.targetdir}/' .. shaders_get_output_path() .. '/%{file.basename}.cso" '
   command = command .. '/O' .. shader.optimizations_level .. ' '

   if shader.debug_enabled then
      command = command .. '/Zi '
    end

   return command
end

function glew_get_path()
   return "External/glew/"
end

function shaders_get_path()
   return "Source/Shaders/"
end

function shaders_get_output_path()
   return "Shaders/"
end

newoption {
   trigger = "renderer",
   value = "API",
   description = "Choose a particular 3D API for rendering",
   allowed = {
      { "opengl", "OpenGL 4" },
      { "d3d11", "Direct3D 11" },
      { "vulkan", "Vulkan" }
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

      -- Compile HLSL shaders
      --    .vs.hlsl Vertex Shader
      --    .ps.hlsl Pixel Shader
      --    .gs.hlsl Geometry Shader
      filter 'files:**.vs.hlsl'
         buildmessage 'Compiling vertex shader %{file.relpath}'

         buildcommands {
            d3d11_compile_shader_command({
               source_path = "%{file.relpath}",
               target_profile = "vs_5_0",
               optimizations_level = 3,
               debug_enabled = true
            })
         }

         cleancommands {
            'rm %{cfg.targetdir}/' .. shaders_get_output_path() .. '/%{file.basename}.cso'
         }

         buildoutputs { '%{cfg.targetdir}/%{file.basename}.cso' }

      filter 'files:**.ps.hlsl'
         buildmessage 'Compiling pixel shader %{file.relpath}'

         buildcommands {
            d3d11_compile_shader_command({
               source_path = "%{file.relpath}",
               target_profile = "ps_5_0",
               optimizations_level = 3,
               debug_enabled = true
            })
         }

         cleancommands {
            'rm %{cfg.targetdir}/' .. shaders_get_output_path() .. '/%{file.basename}.cso'
         }

         buildoutputs { '%{cfg.targetdir}/%{file.basename}.cso' }

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
   entrypoint "WinMainCRTStartup"
   flags { }
   toolset "v141"

   targetdir "Binaries/%{cfg.platform}/%{cfg.buildcfg}"
   objdir "Intermediate/%{cfg.platform}/%{cfg.buildcfg}"
   debugdir "Binaries/%{cfg.platform}/%{cfg.buildcfg}"

   files { "Source/**.h", "Source/**.cpp" }

   prebuildcommands {
      "{MKDIR} %{cfg.targetdir}/Shaders/"
   }

   postbuildcommands {
      "{COPY} %{prj.location}/../Resources/ %{cfg.targetdir}/Resources/"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Off"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"
