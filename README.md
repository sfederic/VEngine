# VEngine - Vagrant Tactics 

![image](https://user-images.githubusercontent.com/45758254/164871735-21b7a40f-fc77-441d-8868-10376bb659dc.png)

## Build Notes
* Visual Studio 2022 17.1
* Blender 2.93.7

## Packages

DirectXTK (directxtk_desktop_win10) ver. 2021.8.2.1 - https://github.com/microsoft/DirectXTK

## Extensions

* Qt VS Tools ver. 2.7.1.20 - https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123
(Currently the engine is using VS2022 tools, the Qt VS extension holds over but VS2022 can't create new Qt projects but builds and settings still work.)
* HLSL Tools for Visual Studio - https://marketplace.visualstudio.com/items?itemName=TimGJones.HLSLToolsforVisualStudio

## Third party libs:

* Physx ver 4.1.2 | Throw PhysX headers/source into Code/Physx - https://github.com/NVIDIAGameWorks/PhysX (used https://vcpkg.io/en/index.html to download SDK [package name is physx:x64-windows])

* FBXSDK ver. 2020.1 | Throw FBXSDK headers/source into Code/FbxSdk - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1

* Qt ver. 5.15.0 - https://download.qt.io/official_releases/qt/5.15/

(The ImGui libs are already included in the repo)
* Dear ImGui - https://github.com/ocornut/imgui
* ImGuizmo - https://github.com/CedricGuillemet/ImGuizmo
