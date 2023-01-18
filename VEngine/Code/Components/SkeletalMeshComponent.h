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

	Skeleton* GetSkeleton() { return skeleton; }

	std::string GetCurrentAnimationName() { return currentAnimation; }
	void SetCurrentAnimationName(std::string currentAnimation_) { currentAnimation = currentAnimation; }
	std::string GetNextAnimationName() { return nextAnimation; }

	float GetCurrentAnimationTime() { return currentAnimationTime; }
	void ResetAnimationTime() { currentAnimationTime = 0.f; }
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment; }

private:
	Skeleton* skeleton = nullptr;
	std::string currentAnimation;
	std::string nextAnimation;
	float currentAnimationTime = 0.f;
};
