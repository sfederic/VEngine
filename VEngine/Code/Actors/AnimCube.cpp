#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent("Skeleton", SkeletalMeshComponent("spin_cubes.vmesh", "test.png"));
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	skeletalMesh->SetAnimationToLoad("spin_cubes@spin.vanim");
	skeletalMesh->CreateJointSocket("gun", "Bone");
}

void AnimCube::Start()
{
	skeletalMesh->PlayAnimation("spin");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
