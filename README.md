# VEngine

## Engine Overview 
The purpose of this repository is to build a beginner to intermediate level game engine with the intent of documenting all the more minute parts of engine as a reference and/or tutorial for onlooking users. The engine itself is being used to create a low-poly, grid-based RPG to be used as a base for the technical aspects of the engine so as to make technical decisions fall within a manageable scope and not push too far in trying to mimic AAA game development techniques.

The engine attempts to keep itself within the C++ realm, opting for several more modern approaches of the language in more of a learning attempt for more recent C++ language standards and concepts. Along with that approach is the attempt to keep the engine purely in C/C++ and not involve outside libraries or languages (either scripting or markup) when dealing with implementations around reflection, serialisation or memory allocation.

![image](https://user-images.githubusercontent.com/45758254/164871735-21b7a40f-fc77-441d-8868-10376bb659dc.png)

## Editor

## Actors and Components

## Serialisation

## Rendering

## Lighting

## Particles

## Animation

## Audio

## User Interface (Gameplay)

## FBX Importing

## Build Notes
* Visual Studio 2022 17.3
* Blender 2.93.7

## Packages

DirectXTK (directxtk_desktop_win10) ver. 2021.8.2.1 - https://github.com/microsoft/DirectXTK

## Extensions

* Qt VS Tools ver. 2.8.0 - https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022
* HLSL Tools for Visual Studio - https://marketplace.visualstudio.com/items?itemName=TimGJones.HLSLToolsforVisualStudio

## Third party libs:

* Physx ver 4.1.2 | Throw PhysX headers/source into Code/Physx - https://github.com/NVIDIAGameWorks/PhysX (used https://vcpkg.io/en/index.html to download SDK [package name is physx:x64-windows])

* FBXSDK ver. 2020.1 | Throw FBXSDK headers/source into Code/FbxSdk - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-1

* DirectXMath | Spherical Harmonics (included in repo) https://github.com/microsoft/DirectXMath/tree/main/SHMath

* Qt ver. 5.15.0 - https://download.qt.io/official_releases/qt/5.15/

(The ImGui libs are already included in the repo)
* Dear ImGui - https://github.com/ocornut/imgui
* ImGuizmo - https://github.com/CedricGuillemet/ImGuizmo
