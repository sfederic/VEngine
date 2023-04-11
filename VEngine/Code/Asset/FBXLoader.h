#pragma once

#include <string>
#include <vector>
#include <map>
#include "Animation/Animation.h"

//The oficial docs on the FBX SDK (the current version is hard to find, google brings you to 2014 docs)
//Ref: https://help.autodesk.com/view/FBX/2020/ENU/

//Great GameDev.net reference on using FBX SDK and animation.
//Ref: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582/

class Skeleton;
struct MeshData;

namespace FBXLoader
{
	void Init();

	//For importing generic fbx assets
	void ImportAsMesh(std::string filepath, MeshData& meshData);

	Animation ImportAsAnimation(const std::string filepath, const std::string filename);

	//For importing cell fractured meshes
	void ImportFracturedMesh(std::string filepath, std::vector<MeshData>& meshDatas);
};
