return {
    kind = "StaticLib",
    links = {
		"GLFW"
	},
    defines = {
		"_LIB"
	},
	directories = {
		include = {
			"./Engine/External/GLFW/Source/"
		}
	}
}