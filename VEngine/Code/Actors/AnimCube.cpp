#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SocketMeshComponent.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent("Skeleton", SkeletalMeshComponent("anim_cube_3.vmesh", "test.png"));
	rootComponent = skeletalMesh;
}

void AnimCube::PostCreate()
{
	skeletalMesh->LoadAnimation("anim_cube_3@move.vanim");
}

void AnimCube::Start()
{
	skeletalMesh->PlayAnimation("move");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
