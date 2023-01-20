#include "vpch.h"
#include "SkeletalMeshComponent.h"

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
