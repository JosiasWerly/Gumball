workspace "Gumball"
	startproject "Gumball"

	configurations { 
		"Debug" 
	}
	architecture "x64"




----------------------DEPENDENCIES----------------------

Projects = {
	"GLAD", "GLFW",
	"GLM", "ImGui",
	"Json", "Lua",
	"Misc"
}

group "Dependecies"
for x in pairs(Projects) do
	project(Projects[x])
	location "Dependencies/%{prj.name}"
	kind "StaticLib"
	language "C++"
	
	targetdir "%{wks.location}/Build/%{cfg.buildcfg}/Dependencies"
	objdir "./Build/%{cfg.buildcfg}/Dependencies/Intermediate/%{prj.name}!"
	
	files { 
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.cpp",
	}
	defines {
		"_DEBUG"
	}
	
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		toolset "v143"
		staticruntime "default"
		symbols "on"
end
group ""

project "GLAD" 
	includedirs {
		"%{prj.location}/include/"
	}

project "GLFW"
	includedirs {
		"%{prj.location}/include/"
	}
	removefiles {
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.cpp",
	}
	files { 
		"%{prj.location}/src/context.c",
		"%{prj.location}/src/egl_context.c",
		"%{prj.location}/src/init.c",
		"%{prj.location}/src/input.c",
		"%{prj.location}/src/monitor.c",
		"%{prj.location}/src/osmesa_context.c",
		"%{prj.location}/src/vulkan.c",
		"%{prj.location}/src/wgl_context.c",
		"%{prj.location}/src/win32_init.c",
		"%{prj.location}/src/win32_joystick.c",
		"%{prj.location}/src/win32_monitor.c",
		"%{prj.location}/src/win32_thread.c",
		"%{prj.location}/src/win32_time.c",
		"%{prj.location}/src/win32_window.c",
		"%{prj.location}/src/window.c",
		
		"%{prj.location}/include/glfw3.h",
		"%{prj.location}/include/glfw3native.h",
	}
	defines {
		"_GLFW_WIN32",
		"_CRT_SECURE_NO_WARNINGS",
	}
	
project "GLM"	
	includedirs {
		"%{prj.location}"
	}
	
project "ImGui"
	includedirs {
		"%{wks.location}/Dependencies/GLFW/include",
		"%{wks.location}/Dependencies/Json"
	}
	defines {
		"_LIB"
	}
	
project "Lua"
	libdirs { 
		"$(ProjectDir)library/",
	}
	links { 
		"lua54.lib"
	}

project "Misc"
	includedirs {
		"%{wks.location}/Dependencies/GLM"
	}


----------------------GUMBALL----------------------


Projects = {
	"GumballCore",
	"Gumball",
	"GumballProject",
}

IncludesDir = {
	"$(SolutionDir)Dependencies/GLAD/include",
	"$(SolutionDir)Dependencies/GLFW/include",
	"$(SolutionDir)Dependencies/GLM",
	"$(SolutionDir)Dependencies/Imgui",
	"$(SolutionDir)Dependencies/Misc",
	"$(SolutionDir)Dependencies/Lua",
}

for x in pairs(Projects) do
	project(Projects[x])
	location "%{prj.name}"
	language "C++"
	kind "SharedLib"
	
	targetdir("%{wks.location}/Build/%{cfg.buildcfg}/")
	objdir "./Build/%{cfg.buildcfg}/Intermediate/%{prj.name}!"
	
	includedirs {
		IncludesDir
	}
	files {
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.cpp",
	}
	defines {
		"_DEBUG"
	}
	disablewarnings {
		"4251" --DLL interface warning
	}
	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		toolset "v143"
		staticruntime "default"
		symbols "on"
end

project "Gumball"
	kind "ConsoleApp"
	libdirs {
		"$(OutputPath)"
	}
	links { 
		"GumballCore", 
		"GumballProject",
		"GumballCore.lib",
	}
	defines {
		"GBAPP"
	}
	forceincludes {
		"$(SolutionDir)/GumballCore/Definitions.hpp"
	}
	includedirs{
		"$(SolutionDir)"
	}

project "GumballCore"
	kind "SharedLib"
	links {
		"GLAD", "GLFW",
		"GLM", "ImGui",
		"Lua", "Misc",
		
		"opengl32.lib",
	}
	defines {
		"GBCORE"
	}
	forceincludes {
		"Definitions.hpp"
	}
	
project "GumballProject"
	kind "SharedLib"
	libdirs {
		"$(OutputPath)"
	}
	links {
		"GumballCore",
		"opengl32.lib",
		"GumballCore.lib",
	}
	defines {
		"GBPROJECT"
	}
	forceincludes {
		"$(SolutionDir)/GumballCore/Definitions.hpp"
	}
	includedirs {	
		"$(SolutionDir)"
	}
