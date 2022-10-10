# VEngine

## Engine Overview 

The purpose of this repository is to build a beginner to intermediate level game engine with the intent of documenting all the more minute parts of engine as a reference and/or tutorial for onlooking users. The engine itself is being used to create a low-poly, grid-based RPG to be used as a base for the technical aspects of the engine so as to make technical decisions fall within a manageable scope and not push too far in trying to mimic AAA game development techniques.

The engine attempts to keep itself within the C++ realm, opting for several more modern approaches of the language in more of a learning attempt for more recent C++ language standards and concepts. Along with that approach is the attempt to keep the engine purely in C/C++ and not involve outside libraries or languages (either scripting or markup) when dealing with implementations around reflection, serialisation or memory allocation.

![image](https://user-images.githubusercontent.com/45758254/164871735-21b7a40f-fc77-441d-8868-10376bb659dc.png)

## Editor

The editor and gameplay code are coupled together and not in seperate projects. Everything is refenceable between the two. 
The engine has both editor setups for Qt (https://www.qt.io/) and Win32 (https://learn.microsoft.com/en-us/windows/win32/), with Qt being the main GUI library in use.
For debugging menus, Dear Imgui (https://github.com/ocornut/imgui) is being used, as well as ImGuizmo (https://github.com/CedricGuillemet/ImGuizmo) to handle object placement in the editor.

## Actors and Components

The engine opts for a system-based approach when dealing with Actors and Components in scenes. The idea is to use singleton classes to hold all specific types of an Actor or Component. This idea was taken from Rare (https://www.youtube.com/watch?v=CBP5bpwkO54) in their development of Sea of Theives with Unreal Engine 4.

## Serialisation

Serialisation is handled entirely in C++, using both binary and text-based formats. Properties https://github.com/sfederic/Engine/blob/master/VEngine/Code/Property.h are taken from Actors and Components to serialise their information to both the editor and exported serialised formats. Exported types need to be defined in the classes defined here https://github.com/sfederic/Engine/blob/master/VEngine/Code/Serialiser.h.

## Rendering

Direct3D 11 is the main 3D graphics API being used for rendering. The renderer https://github.com/sfederic/Engine/blob/master/VEngine/Code/Render/Renderer.h is simple, single threaded and doesn't make use of Command Lists.

## Lighting

Lighting in the engine is based on a simple forward rendering model with support for direction, point and spot lights in scenes. 
In addition, a basic Global Illumination solution is present in the engine using Spherical Harmonics (https://github.com/microsoft/DirectXMath/tree/main/SHMath) alongside a cube map taken from nodes in a scene.

## Particles

https://github.com/sfederic/Engine/blob/master/VEngine/Code/Render/SpriteSystem.h
http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

## Audio

For audio, XAudio2 (https://learn.microsoft.com/en-us/windows/win32/xaudio2/xaudio2-apis-portal) is being used as the main backend for all audio related code.
Because information on game audio programming is so limited, the majority of the audio concepts were taken from https://gdcvault.com/play/1022061/How-to-Write-an-Audio

## User Interface (Gameplay)

For the UI code, Direct2D (https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal) and DirectWrite (https://learn.microsoft.com/en-us/windows/win32/directwrite/direct-write-portal) are being used for 2D shape and font rendering respectively.

As a personal note, one thing I feel that major engines aren't great at is creating and managing UI through code. With the implementation in this engine, it follows a very immediate-style like pattern for creating widgets to be used in-game, with those widgets being inhertied from https://github.com/sfederic/Engine/blob/master/VEngine/Code/UI/Widget.h as the base class.

## FBX Importing

Support for FBX file loading is limited in the engine in https://github.com/sfederic/Engine/blob/master/VEngine/Code/Asset/FBXLoader.h but does offer enough that rudimentary meshes with animations can be imported into the editor.

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
