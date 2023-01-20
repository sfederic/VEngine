#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Asset/FBXLoader.h"

#include "Input.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent(SkeletalMeshComponent("anim_cube.fbx", "test.png"), "Skeleton");
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	FBXLoader::ImportAsAnimation("anim_cube@turn.fbx", skeletalMesh->GetSkeleton());
	FBXLoader::ImportAsAnimation("anim_cube@spin.fbx", skeletalMesh->GetSkeleton());
}

void AnimCube::Tick(float deltaTime)
{
	if (Input::GetKeyUp(Keys::Space))
	{
		skeletalMesh->PlayAnimation("turn", 5.5f);
	}
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
