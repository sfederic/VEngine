g_wkslight = require("premake5-modules.premake-wkslight.wkslight")
g_wkslight.workspace = {
	name = "VEngine-wks",
	startproject = "VEngine",
	platforms = { "x64" },
	cppdialect = "C++20",
	libraries = {
		name = "libraries",
		projects = {
			ExampleStaticLib = {
				location = "libraries/ExampleStaticLib",
				includedirs = {
					"%{wks.location}/../libraries/ExampleStaticLib/include",
				},
			},
		},
	},
	projects = {
		"Editor",
		"VEngine",
	},
}
g_wkslight.extras = {
	qt_qpa_platform_plugin_path = "D:\\dev2\\qt-5.15.2\\plugins\\platforms",
}