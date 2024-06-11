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

for _, m in pairs(GetModules("./Engine/External")) do
	DelcModule("Engine/External", m, false)
end

for _, m in pairs(GetModules("./Engine/Modules")) do
	DelcModule("Engine/Modules", m, true)
end

DelcProject(
	{
		group = "Engine",
		name = "Gumball",
		path = "./Engine/Gumball/Source/Gumball",
		kind = "SharedLib",
		links = {
			"GLM",
		},
		defines = {
			"GENGINE"
		},
		files = {
			force = {
				"Definitions.hpp"
			}
		}
	}
)

DelcGumballProject(
	{
		group = "",
		name = "Sandbox",
		path = "./Sandbox",
		kind = "SharedLib",
		links = {
			"GLM",
		},
		defines = {
			"GGAME"
		},
		files = {
			force = {}
		}
	}
)

DelcRuntime(
	{
		group = "Engine",
		name = "Runtime",
		path = "./Engine/Runtime/",
		kind = "ConsoleApp",
		targetName = "G",
		links = {
			"Sandbox", "opengl32.lib"
		},
		defines = {}
	}
)
