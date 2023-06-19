require("premake5-modules.premake-vspropertysheets.vspropertysheets")
require("premake5-wkslight")
workspace(g_wkslight.workspace.name)
	location(path.getbasename(g_wkslight.workspacedir))
	platforms(g_wkslight.workspace.platforms)
	configurations({ "Debug", "Release" })
	characterset("Default")
	cppdialect(g_wkslight.workspace.cppdialect)
	floatingpoint("Default")
	rtti("Default")
	exceptionhandling("Default")
	filter("system:windows")
		systemversion("latest")
	filter("system:linux")
		pic("On")
	filter("action:vs*")
		startproject(g_wkslight.workspace.startproject)
		staticruntime("Off")
		characterset("MBCS")
		flags({ "MultiProcessorCompile" })
		defines({
			"_CRT_SECURE_NO_WARNINGS",
			"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
			"_HAS_STD_BYTE=0",
		})
		--buildoptions({ "/utf-8", "/Zc:char8_t-" })
	filter({ "action:gmake*", "system:not linux", "kind:*App or StaticLib" })
		buildoptions({ "-static" })
	filter("configurations:Debug")
		defines({ "_DEBUG" })
		symbols("On")
		optimize("Off")
	filter("configurations:Release")
		defines({ "NDEBUG" })
		symbols("Off")
		optimize("Speed")
group(g_wkslight.workspace.thirdparty.name)
	for k, v in pairs(g_wkslight.workspace.thirdparty.projects) do
		include(v.location)
	end
group("")
	for i, v in ipairs(g_wkslight.workspace.projects) do
		include(v)
	end