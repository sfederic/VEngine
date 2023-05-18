# VEngine

## Overview
A beginner to intermediate game engine, the purpose of it being open source is to detail overlooked programming concepts and build a reference for onlookers, from programming to design concepts.

As many hobby engines are more focused on graphical and technical implementations, this engine's graphical and technical scopes are simplified in order to cater to more beginner oriented use cases. 

![image](https://github.com/sfederic/Engine/assets/45758254/ae28fc21-0288-45ae-bd82-1c61020f7221)

## Build Notes
* Visual Studio 2022 17.4
* Blender 3.4 (For FBX SDK)

## Packages

DirectXTK (directxtk_desktop_win10) ver. 2021.8.2.1 - https://github.com/microsoft/DirectXTK

## Third party libs:

* Physx ver 4.1.2 | Throw PhysX headers/source into Code/Physx - https://github.com/NVIDIAGameWorks/PhysX (used https://vcpkg.io/en/index.html to download SDK [package name is physx:x64-windows])

* FBXSDK ver. 2020.1 | Throw FBXSDK headers/source into Code/FbxSdk - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1

* DirectXMath | Spherical Harmonics (included in repo) https://github.com/microsoft/DirectXMath/tree/main/SHMath

* Qt ver. 5.15.0 - https://download.qt.io/official_releases/qt/5.15/

(The ImGui libs are already included in the repo)
* Dear ImGui - https://github.com/ocornut/imgui
* ImGuizmo - https://github.com/CedricGuillemet/ImGuizmo

## Helpful Extensions

* Qt VS Tools ver. 2.10.0 - https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
* HLSL Tools for Visual Studio - https://marketplace.visualstudio.com/items?itemName=TimGJones.HLSLToolsforVisualStudio
