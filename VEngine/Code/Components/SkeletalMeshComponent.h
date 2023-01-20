#pragma once

#include "MeshComponent.h"
#include "Render/RenderTypes.h"

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
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment * animationSpeed; }

	Animation& GetCurrentAnimation();
	std::vector<Animation*> GetAllAnimations();

	std::vector<Joint>& GetAllJoints();
	bool HasJoints();

	void PlayAnimation(std::string animationName, float speed = 1.f);

	std::string currentAnimation;

	ShaderSkinningData shaderSkinningData;

private:
	float currentAnimationTime = 0.f;
	float animationSpeed = 1.f;
};
