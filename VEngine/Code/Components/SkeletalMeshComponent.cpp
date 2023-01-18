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
