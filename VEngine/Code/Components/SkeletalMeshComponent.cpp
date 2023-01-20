#include "vpch.h"
#include "SkeletalMeshComponent.h"
#include <algorithm>
#include "Core.h"

Properties SkeletalMeshComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "SkeletonMeshComponent";
    return props;
}

void SkeletalMeshComponent::Create()
{
    MeshComponent::Create();
}

Animation& SkeletalMeshComponent::GetCurrentAnimation()
{
    auto& skeleton = GetSkeleton();
    auto& animation = skeleton.GetAnimation(currentAnimationName);
    return animation;
}

Animation& SkeletalMeshComponent::GetNextAnimation()
{
	auto& skeleton = GetSkeleton();
	auto& animation = skeleton.GetAnimation(nextAnimationName);
	return animation;
}

Animation& SkeletalMeshComponent::GetAnimation(std::string animationName)
{
	auto& skeleton = GetSkeleton();
	auto& animation = skeleton.GetAnimation(animationName);
	return animation;
}

std::vector<Animation*> SkeletalMeshComponent::GetAllAnimations()
{
    std::vector<Animation*> animations;
    for (auto& [name, animation] : GetSkeleton().animations)
    {
        animations.push_back(&animation);
    }
    return animations;
}

std::vector<Joint>& SkeletalMeshComponent::GetAllJoints()
{
    return GetSkeleton().joints;
}

bool SkeletalMeshComponent::HasJoints()
{
    return GetSkeleton().joints.size();
}

void SkeletalMeshComponent::PlayAnimation(std::string animationName, float speed)
{
	animationState = AnimationState::Play;
    currentAnimationName = animationName;
    currentAnimationSpeed = speed;
}

void SkeletalMeshComponent::StopAnimation()
{
	currentAnimationName.clear();
}

void SkeletalMeshComponent::SetPauseAnimationState()
{
	animationState = AnimationState::Pause;
}

void SkeletalMeshComponent::StopAllAnimations()
{
	for (auto& skeletalMesh : system.GetComponents())
	{
		skeletalMesh->SetPauseAnimationState();
	}
}

void SkeletalMeshComponent::StartAllAnimations()
{
	for (auto& skeletalMesh : system.GetComponents())
	{
		skeletalMesh->animationState = AnimationState::Play;
	}
}

void SkeletalMeshComponent::InterpolateCurrentAnimation()
{
	int skinningDataIndex = 0;
	Animation& anim = GetCurrentAnimation();

	if (!anim.frames.empty())
	{
		shaderSkinningData.isAnimated = true;

		switch (animationState)
		{
		case AnimationState::Play: break;
		case AnimationState::Pause: return;
		}

		IncrementAnimationTime(Core::GetDeltaTime());

		//Move through and animate all joints on skeleton
		auto& joints = GetAllJoints();
		for (auto& joint : joints)
		{
			if (GetCurrentAnimationTime() >= anim.GetEndTime(joint.index))
			{
				ResetAnimationTime();
			}

			anim.Interpolate(GetCurrentAnimationTime(), joint, &GetSkeleton());

			shaderSkinningData.skinningMatrices[skinningDataIndex] = joint.currentPose;
			skinningDataIndex++;
			assert(skinningDataIndex < ShaderSkinningData::MAX_SKINNING_DATA);
		}
	}
	else
	{
		shaderSkinningData.isAnimated = false;
	}
}

void SkeletalMeshComponent::CrossFadeNextAnimation()
{
	Animation& animA = GetCurrentAnimation();
	Animation& animB = GetNextAnimation();

	//If crossfade is set to same animation, skip entire fade
	if (animA.name == animB.name)
	{
		blendFactor = 0.f;
		currentAnimationName = nextAnimationName;
		nextAnimationName.clear();
		Log("Cross fade for %s animation skipped for being the same Animation", animA.name.c_str());
		return;
	}

	int skinningDataIndex = 0;

	const float currentAnimTime = GetCurrentAnimationTime();

	auto& joints = GetAllJoints();
	for (auto& joint : joints)
	{
		std::vector<AnimFrame>& jointFramesAnimA = animA.frames[joint.index];
		std::vector<AnimFrame>& jointFramesAnimB = animB.frames[joint.index];

		for (size_t i = 0; i < (jointFramesAnimA.size() - 1); i++)
		{
			constexpr float blendSpeed = 0.077f;
			blendFactor += Core::GetDeltaTime() * blendSpeed;
			if (blendFactor > 1.f)
			{
				blendFactor = 0.f;
				currentAnimationName = nextAnimationName;
				nextAnimationName.clear();
				ResetAnimationTime();
				return;
			}

			const float lerpPercent = std::clamp(blendFactor, 0.f, 1.f);

			if (currentAnimTime >= jointFramesAnimA[i].time)
			{
				XMVECTOR currentPos = XMLoadFloat3(&jointFramesAnimA[i].pos);
				XMVECTOR currentScale = XMLoadFloat3(&jointFramesAnimA[i].scale);
				XMVECTOR currentRot = XMLoadFloat4(&jointFramesAnimA[i].rot);

				XMVECTOR nextPos = XMLoadFloat3(&jointFramesAnimA[0].pos);
				XMVECTOR nextScale = XMLoadFloat3(&jointFramesAnimA[0].scale);
				XMVECTOR nextRot = XMLoadFloat4(&jointFramesAnimA[0].rot);

				XMVECTOR lerpedPos = XMVectorLerp(currentPos, nextPos, lerpPercent);
				XMVECTOR lerpedScale = XMVectorLerp(currentScale, nextScale, lerpPercent);
				XMVECTOR lerpedRot = XMQuaternionSlerp(currentRot, nextRot, lerpPercent);
				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				joint.currentPose = XMMatrixAffineTransformation(lerpedScale, zero, lerpedRot, lerpedPos);

				XMMATRIX endPose = joint.currentPose;
				int parentIndex = joint.parentIndex;

				while (parentIndex > -1)
				{
					Joint& parentJoint = GetSkeleton().joints[parentIndex];
					endPose *= parentJoint.currentPose;

					parentIndex = parentJoint.parentIndex;
				}

				joint.currentPose = joint.inverseBindPose * endPose;
			}
		}

		shaderSkinningData.skinningMatrices[skinningDataIndex] = joint.currentPose;
		skinningDataIndex++;
		assert(skinningDataIndex < ShaderSkinningData::MAX_SKINNING_DATA);
	}
}

void SkeletalMeshComponent::SetCrossFade(std::string animationNameToBlendTo)
{
	nextAnimationName = animationNameToBlendTo;
}
