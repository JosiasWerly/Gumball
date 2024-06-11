return {
	kind = "StaticLib",
	defines = {
		"_GLFW_WIN32",
		"_CRT_SECURE_NO_WARNINGS",
	},
	files = {
		absolute = {},
		relative = {
			"context.c",
			"egl_context.c",
			"init.c",
			"input.c",
			"monitor.c",
			"osmesa_context.c",
			"vulkan.c",
			"wgl_context.c",
			"win32_init.c",
			"win32_joystick.c",
			"win32_monitor.c",
			"win32_thread.c",
			"win32_time.c",
			"win32_window.c",
			"window.c",
			"glfw3.h",
			"glfw3native.h",
		}
	}
}