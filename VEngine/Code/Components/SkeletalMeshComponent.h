#pragma once

#include "MeshComponent.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(SkeletalMeshComponent);

	SkeletalMeshComponent() {}
	SkeletalMeshComponent(std::string meshFilename, std::string textureFilename) :
		MeshComponent(meshFilename, textureFilename) {}

	Properties GetProps() override;

	Skeleton* skeleton = nullptr;

	std::string currentAnimation;
	std::string nextAnimation;

	float currentAnimationTime = 0.f;
};
