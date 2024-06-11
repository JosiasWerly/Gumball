----------------- Module template -----------------
-- kind = "",
-- links = {},
-- defines = {},
-- files = {
-- 	absolute = {},
-- 	relative = {},
-- 	force = {}
-- },
-- directories = {
-- 	include = {},
-- 	library = {}
-- }
----------------------------------------------------

local moduleTemplate = [[
return {
	kind = "StaticLib",
	links = {},
	defines = {},
	files = {
		absolute = {},
		relative = {},
		force = {}
	},
	directories = {
		include = {},
		library = {}
	}
}]]

externalModules = {}
engineModules = {}
function Contains(array, value)
	for _, v in ipairs(array) do
		if v == value then
			return true
		end
	end
	return false
end

function HasFileAt(path)
	local file = io.open(path, "r")
	if file then
		file:close()
		return true
	else
		return false
	end
end

function GetFilesByExtension(path, extension)
	local files = {}
	local handle = io.popen('dir "' .. path .. '" /b /a-d')
	if handle then
		for file in handle:lines() do
			if file:match("%." .. extension .. "$") then
				table.insert(files, file)
			end
		end
		handle:close()
	end
	return files
end

function GetModules(path)
	local modules = {}
	local handle = io.popen('dir "' .. path .. '" /b /ad')
	if handle then
		for folder in handle:lines() do
			local modName = folder
			local modPath = path .. "/" .. folder .. "/"
			local modMake = modPath .. modName .. ".make.lua"

			if HasFileAt(modMake) == false then
				local file = io.open(modMake, "w")
				if file then
					file:write(moduleTemplate)
					file:close()
				end
			end

			if HasFileAt(modMake) then
				local modMakeInfo = dofile(modMake)
				modMakeInfo.group = ""
				modMakeInfo.name = modName
				modMakeInfo.path = modPath
				modMakeInfo.make = modMake
				modules[modName] = modMakeInfo
			else
				print("unable to generate" .. modMake)
			end
		end
		handle:close()
	end
	return modules
end

function DelcValidation(trg)
	--i'm not proud of this function, i'm tired and I would like to never see this again.
	local ls = {
		"group", "name",
		"path", "kind"
	}
	for _, v in ipairs(ls) do
		if not trg[v] then
			print(trg.name .. " missing " .. v)
			return false;
		end
	end

	if not trg.links then
		trg.links = {}
	end

	if not trg.defines then
		trg.defines = {}
	end

	if not trg.files then
		trg.files = {
			absolute = {},
			relative = {},
			force = {}
		}
	else
		if not trg.files.absolute then
			trg.files.absolute = {}
		end
		if not trg.files.relative then
			trg.files.relative = {}
		end
		if not trg.files.force then
			trg.files.force = {}
		end
	end

	if not trg.directories then
		trg.directories = {
			include = {},
			library = {}
		}
	else
		if not trg.directories.include then
			trg.directories.include = {}
		end
		if not trg.directories.library then
			trg.directories.library = {}
		end
	end
	return true
end

function Delc(projInfo)
	-- {
	-- 	group = "",
	-- 	name = "Gumball",
	-- 	path = "./Engine/Gumball/",
	-- 	kind = "ConsoleApp",
	-- 	links = {},
	-- 	defines = {},
	-- 	files = {
	-- 		absolute = {},
	-- 		relative = {},
	--		force = {}
	-- 	},
	-- 	directories = {
	-- 		include = {},
	-- 		library = {}
	-- 	}
	-- }
	if DelcValidation(projInfo) == false then
		print("not declared " .. projInfo.name)
	end

	group(projInfo.group)
	project(projInfo.name)
	location(projInfo.path)
	kind(projInfo.kind)
	defines(projInfo.defines)
	includedirs(projInfo.path)

	if #projInfo.files.absolute ~= 0 then
		for _, i in pairs(projInfo.files.absolute) do
			files(i)
		end
	end

	if #projInfo.files.force ~= 0 then
		for _, i in pairs(projInfo.files.force) do
			forceincludes(i)
		end
	end

	if #projInfo.files.relative == 0 then
		files {
			"%{prj.location}/**.h",
			"%{prj.location}/**.c",
			"%{prj.location}/**.hpp",
			"%{prj.location}/**.cpp",
		}
	else
		for _, i in pairs(projInfo.files.relative) do
			files("%{prj.location}/" .. i)
		end
	end

	for _, i in pairs(projInfo.directories.include) do
		includedirs(i)
	end

	for _, i in pairs(projInfo.directories.library) do
		libdirs(i)
		local libArr = GetFilesByExtension(i, "lib")
		for _, i in pairs(libArr) do
			table.insert(projInfo.links, i)
		end
	end

	for _, i in pairs(projInfo.links) do
		if not string.find(i, ".lib") then
			if Contains(externalModules, i) then
				includedirs("%{wks.location}/Engine/External/" .. i .. "/Source/")
			else
				includedirs("%{wks.location}/Engine/Modules/" .. i .. "/Source/")
			end
		end
		links(i)
	end

	disablewarnings {
		"4251", -- DLL interface
		"4275", -- DLL class inheritance
		"4005" -- delc macro multiple times
	}

	language("C++")
	if projInfo.targetName then
		targetname(projInfo.targetName)
	end
	targetdir("%{wks.location}/Binaries/%{cfg.buildcfg}/")
	objdir("%{wks.location}/Binaries/%{cfg.buildcfg}/obj/%{prj.name}!")

	filter("system:windows")
	cppdialect "C++20"
	systemversion "latest"
	toolset "v143"
	staticruntime "default"
	symbols "on"
	group("")
end

function GenerateModulesFile(trg)
	table.sort(engineModules, function(a, b) return a:upper() < b:upper() end)
	local file = io.open(trg.path .. "modules.gen.cpp", "w")
	if file then
		file:write("#include \"Runtime.hpp\"\n")
		file:write("#include <Gumball/Engine.hpp>\n")
		for _, name in ipairs(engineModules) do
			file:write("#include <" .. name .. "/" .. name .. ".module.hpp>\n")
		end
		file:write("void injectModules(ModuleController *mCtrl) {\n")		
		for _, name in ipairs(engineModules) do
			file:write("\tmCtrl->addModule<".. name .. "Module>();\n")
		end
		file:write("}")
		file:close()
	end
	-- table.sort(engineModules, function(a, b) return a:upper() < b:upper() end)

	-- local path = trg.path
	-- local file = io.open(path .. "Modules.hpp", "w")
	-- if file then
	-- 	file:write("#include <list>\nstd::list<Module*> getModules();")
	-- 	file:close()
	-- end

	-- local file = io.open(path .. "Modules.cpp", "w")
	-- if file then
	-- 	file:write("#include \"Modules.hpp\"\n")
	-- 	local includes = [[]]
	-- 	for _, name in ipairs(engineModules) do
	-- 		includes = includes .. "#include <" .. name .. "/" .. name .. ".module.hpp>\n"
	-- 	end
	-- 	file:write(includes)

	-- 	file:write("std::list<Module*> getModules() {\n\treturn {\n")
	-- 	for _, name in ipairs(engineModules) do
	-- 		file:write("\t\tnew " .. name .. "Module,\n")
	-- 	end
	-- 	file:write("\t};\n}")
	-- 	file:close()
	-- end
end

function DelcProject(projInfo)
	if DelcValidation(projInfo) == false then
		return
	end

	Delc(projInfo)
end

function DelcModule(groupName, moduleInfo, isEngine)
	local modPath = moduleInfo["path"]
	moduleInfo.group = groupName
	moduleInfo.path = moduleInfo.path .. "Source/" .. moduleInfo.name .. "/"

	if DelcValidation(moduleInfo) == false then
		return
	end

	if isEngine then
		InjectGumball(moduleInfo)
		table.insert(engineModules, moduleInfo.name)
		table.insert(moduleInfo.defines, "GMODULE")
	else
		table.insert(externalModules, moduleInfo.name)
	end

	table.insert(moduleInfo.directories.library, modPath .. "Libraries/")
	table.insert(moduleInfo.directories.include, modPath .. "Source/")
	Delc(moduleInfo)
end

function DelcRuntime(trg)
	DelcValidation(trg)
	for _, v in ipairs(engineModules) do
		table.insert(trg.directories.include, "./Engine/" .. v .. "/Source/")
		table.insert(trg.links, v)
	end
	InjectGumball(trg)
	GenerateModulesFile(trg)
	DelcProject(trg)
end

function DelcGumballProject(projInfo)
	DelcValidation(projInfo)
	InjectEngineModules(projInfo)
	InjectGumball(projInfo)
	DelcProject(projInfo)
end

function InjectGumball(trg)
	table.insert(trg.directories.include, "$(SolutionDir)/Engine/Gumball/Source/")
	table.insert(trg.files.force, "$(SolutionDir)/Engine/Gumball/Source/Gumball/Gumball.hpp")
	table.insert(trg.links, "Gumball")
	table.insert(trg.links, "GLM")
end

function InjectEngineModules(trg)
	for _, name in ipairs(engineModules) do
		table.insert(trg.links, name)
	end
end