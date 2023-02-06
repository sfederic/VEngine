#pragma once

#include "MeshComponent.h"

//Mesh that can be sliced via plane and segmented into seperate meshes.
class SliceableMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(SliceableMeshComponent);

	SliceableMeshComponent() {}
	SliceableMeshComponent(std::string meshFilename, std::string textureFilename) :
		MeshComponent(meshFilename, textureFilename) {}

	void Create() override { MeshComponent::Create(); }

	void SliceMesh(XMVECTOR planeCenter, XMVECTOR planeNormal);
};
