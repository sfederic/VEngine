#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent(SkeletalMeshComponent("spin_cubes.vmesh", "test.png"), "Skeleton");
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	skeletalMesh->LoadAnimation("spin_cubes@spin.fbx");
}

void AnimCube::Start()
{
	skeletalMesh->PlayAnimation("spin");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
