#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SocketMeshComponent.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent("Skeleton", SkeletalMeshComponent("spin_cubes.vmesh", "test.png"));
	rootComponent = skeletalMesh;

	socketMesh = CreateComponent("Socket", SocketMeshComponent(0, "ico_sphere.vmesh", "test.png"));
	socketMesh->LinkToSkeletalMeshComponent(skeletalMesh);
}

void AnimCube::PostCreate()
{
	skeletalMesh->LoadAnimation("spin_cubes@spin.vanim");
}

void AnimCube::Start()
{
	skeletalMesh->PlayAnimation("spin");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
