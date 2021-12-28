#pragma once
#include "MeshComponent.h"

//Ref: https://80.lv/articles/creating-a-destructible-object-in-unreal-engine-4-blender/

struct DestructibleMeshComponent : MeshComponent
{
	COMPONENT_SYSTEM(DestructibleMeshComponent);

	std::vector<MeshData> meshDatas;

	DestructibleMeshComponent(const std::string filename_,
		const std::string textureFilename_,
		const std::string shaderFilename_ = "DefaultShader.hlsl");
	virtual void Create() override;
	virtual Properties GetProps() override;
};
