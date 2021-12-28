#pragma once
#include "MeshComponent.h"

//Destructible mesh workflow:
// 1. Blender -> Edit -> Preferences : turn on cell fracture
// 2. F3 on the selected mesh, search 'Cell Fracture'
// 3. fiddle around with the settings, make it random
// 4. FBXLoader::ImportFracturedMesh() is only working with meshes that have no children in the fbx
//Ref: https://80.lv/articles/creating-a-destructible-object-in-unreal-engine-4-blender/

//@Todo: destructible meshes are "working", but not well. Maybe come back and fix it all up later.

struct DestructibleMeshComponent : MeshComponent
{
	COMPONENT_SYSTEM(DestructibleMeshComponent);

	std::vector<MeshData> meshDatas;
	std::vector<MeshComponent*> meshCells;

	DestructibleMeshComponent(const std::string filename_,
		const std::string textureFilename_,
		const std::string shaderFilename_ = "DefaultShader.hlsl");
	virtual void Create() override;
	virtual Properties GetProps() override;
};
