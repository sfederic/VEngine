#include "vpch.h"
#include "AnimCube.h"
#include "Components/MeshComponent.h"
#include "Asset/FBXLoader.h"

AnimCube::AnimCube()
{
	mesh = CreateComponent(MeshComponent("anim_cube.fbx", "test.png"), "Skeleton");
	rootComponent = mesh;
}

void AnimCube::Create()
{
	FBXLoader::ImportAsAnimation("anim_cube@turn.fbx", mesh->meshDataProxy);
	FBXLoader::ImportAsAnimation("anim_cube@spin.fbx", mesh->meshDataProxy);
}

Properties AnimCube::GetProps()
{
	return __super::GetProps();
}
