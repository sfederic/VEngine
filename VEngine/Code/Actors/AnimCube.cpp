#include "vpch.h"
#include "AnimCube.h"
#include "Components/SkeletalMeshComponent.h"
#include "Asset/FBXLoader.h"

AnimCube::AnimCube()
{
	skeletalMesh = CreateComponent(SkeletalMeshComponent("anim_cube.fbx", "test.png"), "Skeleton");
	rootComponent = skeletalMesh;
}

void AnimCube::Create()
{
	FBXLoader::ImportAsAnimation("anim_cube@turn.fbx", *skeletalMesh->skeleton);
	FBXLoader::ImportAsAnimation("anim_cube@spin.fbx", *skeletalMesh->skeleton);
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
