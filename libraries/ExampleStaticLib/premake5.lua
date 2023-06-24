project("ExampleStaticLib")
	location(g_wkslight.workspacedir .. "/%{prj.name}")
	targetdir(g_wkslight.targetdir)
	kind("StaticLib")
	language("C++")
	files({
		"include/**.h",
		"src/**.cpp",
	})
	includedirs({
		g_wkslight.workspace.libraries.projects.ExampleStaticLib.includedirs,
	})