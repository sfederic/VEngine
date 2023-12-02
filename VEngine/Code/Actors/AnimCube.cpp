#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SocketMeshComponent.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent<SkeletalMeshComponent>("Skeleton");
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	skeletalMesh->SetMeshFilename("cube_left_right.vmesh");
	skeletalMesh->SetTexture("test.png");
}

void AnimCube::PostCreate()
{
	skeletalMesh->LoadAnimations("cube_left_right.vanim");
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
