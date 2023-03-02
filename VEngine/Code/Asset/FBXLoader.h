#pragma once

#include <string>
#include <vector>
#include <map>
#include "Render/RenderTypes.h"

//The oficial docs on the FBX SDK (the current version is hard to find, google brings you to 2014 docs)
//Ref: https://help.autodesk.com/view/FBX/2020/ENU/

//Great GameDev.net reference on using FBX SDK and animation.
//Ref: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582/

struct Skeleton;

namespace FBXLoader
{
	void Init();

	//For importing generic fbx assets
	void ImportAsMesh(std::string filename, MeshData& meshData);

	Animation ImportAsAnimation(const std::string filename);

	//For importing cell fractured meshes
	void ImportFracturedMesh(std::string filename, std::vector<MeshData>& meshDatas);
};
