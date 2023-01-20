#include "vpch.h"
#include "SkeletalMeshComponent.h"
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
    auto& animation = skeleton.GetAnimation(currentAnimation);
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
    currentAnimation = animationName;
    animationSpeed = speed;
}

void SkeletalMeshComponent::PauseAnimation()
{
	animationSpeed = 0.f;
}

void SkeletalMeshComponent::PauseAllAnimations()
{
	for (auto& skeletalMesh : system.GetComponents())
	{
		skeletalMesh->PauseAnimation();
	}
}

void SkeletalMeshComponent::InterpolateCurrentAnimation()
{
	int skinningDataIndex = 0;
	Animation& anim = GetCurrentAnimation();

	if (!anim.frames.empty())
	{
		shaderSkinningData.isAnimated = true;

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
