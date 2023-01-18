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

	std::string GetCurrentAnimationName() { return currentAnimation; }
	void SetCurrentAnimationName(std::string currentAnimation_) { currentAnimation = currentAnimation_; }
	std::string GetNextAnimationName() { return nextAnimation; }

	float GetCurrentAnimationTime() { return currentAnimationTime; }
	void ResetAnimationTime() { currentAnimationTime = 0.f; }
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment; }

private:
	std::string currentAnimation;
	std::string nextAnimation;

	float currentAnimationTime = 0.f;
};
