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
	void Create() override;

	Skeleton& GetSkeleton() { return *meshDataProxy.skeleton; }

	float GetCurrentAnimationTime() { return currentAnimationTime; }
	void ResetAnimationTime() { currentAnimationTime = 0.f; }
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment; }

	Animation& GetCurrentAnimation();

	std::string currentAnimation;
	std::string nextAnimation;

private:
	float currentAnimationTime = 0.f;
};
