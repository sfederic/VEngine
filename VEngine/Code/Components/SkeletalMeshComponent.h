#pragma once

#include "MeshComponent.h"
#include "Animation/Joint.h"
#include "Render/ShaderData/ShaderSkinningData.h"

class Animation;

//Great reference on implementation of animation systems.
//Ref:https://blog.demofox.org/2012/09/21/anatomy-of-a-skeletal-animation-system-part-1/

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

	Skeleton& GetSkeleton();

	void LoadAnimation(std::string animationFilename);
	float GetCurrentAnimationTime() { return currentAnimationTime; }
	void ResetAnimationTime();
	void IncrementAnimationTime(float increment) { currentAnimationTime += increment * currentAnimationSpeed; }
	void SetAnimationSpeed(float animationSpeed) { currentAnimationSpeed = animationSpeed; }

	std::string GetCurrentAnimatonName() { return currentAnimationName; }
	std::string GetNextAnimationName() { return nextAnimationName; }

	Animation& GetCurrentAnimation();
	Animation& GetNextAnimation();
	Animation& GetAnimation(std::string animationName);
	std::vector<Animation*> GetAllAnimations();

	std::vector<Joint>& GetAllJoints();
	bool HasJoints();
	int GetJointIndexByName(const std::string boneName);

	void PlayAnimation(std::string animationName, float speed = 1.f, bool loop = true);
	void StopAnimation();
	void SetPauseAnimationState();
	void InterpolateCurrentAnimation();

	//This cross fade implementation takes the current animation time point of the animation to fade from and
	//fades into the first frame of the animation to fade into, meaning there's no increment of time from the
	//animation to fade from.
	void CrossFadeNextAnimation();

	void SetCrossFade(std::string animationNameToBlendTo);

	ShaderSkinningData shaderSkinningData;

private:
	std::vector<std::string> animationsToLoadOnCreate;

	AnimationState animationState = AnimationState::Play;

	std::string currentAnimationName;
	std::string nextAnimationName;

	float currentAnimationTime = 0.f;
	float currentAnimationSpeed = 1.f;

	float blendFactor = 0.f;

	bool isAnimationLooping = true;
};
