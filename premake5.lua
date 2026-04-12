dofile("common.lua")

workspace "Gumball"
startproject "Runtime"
configurations {
	"Debug", "Release"
}
architecture "x64"

filter "configurations:Debug"
defines { "DEBUG" }
optimize "Off"
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines { "NDEBUG", "CURL_STATICLIB" }
optimize "On"
symbols "Off"
runtime "Release"

filter {}

-- maps all modules that are considered external, mostly things that we don't have ownership of it
for _, m in pairs(GetModules("./Engine/External")) do
	DelcModule("Engine/External", m, false)
end

-- maps all internal gumball modules, things that we coded 100%
for _, m in pairs(GetModules("./Engine/Modules")) do
	DelcModule("Engine/Modules", m, true)
end

-- generates gumball core a.k.a gumball.dll, note that gumball project doesn't "know" internal modules
DelcProject(
	{
		group = "Engine",
		name = "Gumball",
		path = "./Engine/Gumball/Source/Gumball",
		kind = "SharedLib",
		links = { "GLM", "Json" },
		defines = { "GENGINE" },
		files = {
			force = { "%{prj.location}/Framework/Definitions.hpp", }
		},
		directories = {
			include = { "./Engine/Gumball/Source/" }
		}
	}
)

-- this is gumball application which binds gumball.dll and each module as static lib, injecting each module into gumball core
DelcRuntime(
	{
		group = "Engine",
		name = "Runtime",
		path = "./Engine/Runtime/",
		kind = "ConsoleApp",
		targetName = "G",
		links = { "Sandbox", "opengl32.lib" },
		defines = {}
	}
)

--sandbox project as example
DelcGumballProject(
	{
		group = "",
		name = "Sandbox",
		path = "./Sandbox",
		kind = "SharedLib",
		links = { "GLM", },
		defines = { "GGAME" },
		files = {
			force = {}
		}
	}
)