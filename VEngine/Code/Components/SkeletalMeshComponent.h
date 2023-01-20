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
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment * animationSpeed; }

	Animation& GetCurrentAnimation();
	std::vector<Animation*> GetAllAnimations();

	std::vector<Joint>& GetAllJoints();
	bool HasJoints();

	void PlayAnimation(std::string animationName, float speed = 1.f);
	void StopAnimation();
	void SetPauseAnimationState();

	void InterpolateCurrentAnimation();

	std::string currentAnimation;

	ShaderSkinningData shaderSkinningData;

private:
	AnimationState animationState = AnimationState::Play;

	float currentAnimationTime = 0.f;
	float animationSpeed = 1.f;
};
