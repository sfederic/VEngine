project("VEngine")
	location(g_wkslight.workspacedir .. "/%{prj.name}")
	targetdir(g_wkslight.targetdir)
	debugdir("")
	debugargs({ "" })
	kind("ConsoleApp")
	language("C++")
	pchheader("vpch.h")
	pchsource("Code/vpch.cpp")
	files({
		"Code/**.h",
		"Code/**.cpp",
		"Code/**.hlsl",
	})
	removefiles({
		"Code/Editor/imgui/backends/**.*",
		"Code/Editor/imgui/examples/**.*",
		"Code/Editor/imgui/misc/**.*",
		"Code/Editor/ImGuizmo/example/**.*",
		"Code/Editor/ImGuizmo/vcpkg-example/**.*",
	})
	files({
		"Code/Editor/imgui/backends/imgui_impl_dx11.cpp",
		"Code/Editor/imgui/backends/imgui_impl_win32.cpp",
	})
	includedirs({
		"Code",
		"Code/Editor/imgui",
		"Code/Editor/ImGuizmo",
	})
	links({
		"d3d11.lib",
		"dxgi.lib",
		"d3dcompiler.lib",
		"d2d1.lib",
		"dwrite.lib",
	})
	defines({
		--"_USE_MATH_DEFINES",
	})
	filter("files:Code/Editor/imgui/**.cpp")
		flags({ "NoPCH" })
	filter("files:Code/Editor/ImGuizmo/**.cpp")
		flags({ "NoPCH" })
	filter("files:Code/SHMath/**.cpp")
		flags({ "NoPCH" })
	filter("files:Code/**.hlsl")
		flags({ "ExcludeFromBuild" })
		shadermodel("5.0")
		shaderobjectfileoutput("%{wks.location}/../%{prj.name}/Shaders/%{file.basename}.cso")
	filter("files:Code/Render/Shaders/Vertex/*.hlsl")
		removeflags({ "ExcludeFromBuild" })
		shadertype("Vertex")
		shaderobjectfileoutput("%{wks.location}/../%{prj.name}/Shaders/Vertex/%{file.basename}.cso")
	filter("files:Code/Render/Shaders/Pixel/*.hlsl")
		removeflags({ "ExcludeFromBuild" })
		shadertype("Pixel")
		shaderobjectfileoutput("%{wks.location}/../%{prj.name}/Shaders/Pixel/%{file.basename}.cso")
	filter("files:Code/Render/Shaders/Compute/*CS.hlsl")
		removeflags({ "ExcludeFromBuild" })
		shadertype("Compute")
		shaderentry("CSFunc")
		--shaderoptions({ "/E 'CSFunc'" })
	filter("action:vs*")
		debugenvs({
			"$(LocalDebuggerEnvironment)",
			"QT_QPA_PLATFORM_PLUGIN_PATH=" .. g_wkslight.extras.qt_root .. "\\plugins\\platforms",
		})
		vspropertysheet({
			"D:\\dev2\\DirectXTK.props",
			"D:\\dev2\\fbxsdk.props",
			"D:\\dev2\\PhysX-4.1.props",
			"D:\\dev2\\qt5.props",
		})