#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Asset/FBXLoader.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent(SkeletalMeshComponent("spin_cubes.fbx", "test.png"), "Skeleton");
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	FBXLoader::ImportAsAnimation("spin_cubes@spin.fbx", skeletalMesh->GetSkeleton());
}

void AnimCube::Start()
{
	skeletalMesh->PlayAnimation("spin");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
