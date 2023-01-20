#pragma once

#include "MeshComponent.h"
#include "Render/RenderTypes.h"

enum class AnimationState
{
	Play,
	Pause
};

class SkeletalMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(SkeletalMeshComponent);

	//Pauses all SkeletalMeshComponents across system
	static void StopAllAnimations();

	//Plays all SkeletalMeshComponents across system
	static void StartAllAnimations();

	SkeletalMeshComponent() {}
	SkeletalMeshComponent(std::string meshFilename, std::string textureFilename) :
		MeshComponent(meshFilename, textureFilename) {}

	Properties GetProps() override;
	void Create() override;

	Skeleton& GetSkeleton() { return *meshDataProxy.skeleton; }

	float GetCurrentAnimationTime() { return currentAnimationTime; }
	void ResetAnimationTime() { currentAnimationTime = 0.f; }
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment * currentAnimationSpeed; }
	void SetAnimationSpeed(float animationSpeed) { currentAnimationSpeed = animationSpeed; }

	Animation& GetCurrentAnimation();
	std::string GetCurrentAnimatonName() { return currentAnimationName; }
	std::vector<Animation*> GetAllAnimations();

	std::vector<Joint>& GetAllJoints();
	bool HasJoints();

	void PlayAnimation(std::string animationName, float speed = 1.f);
	void StopAnimation();
	void SetPauseAnimationState();

	void InterpolateCurrentAnimation();

	ShaderSkinningData shaderSkinningData;

private:
	AnimationState animationState = AnimationState::Play;

	std::string currentAnimationName;

	float currentAnimationTime = 0.f;
	float currentAnimationSpeed = 1.f;
};
