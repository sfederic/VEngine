#include "vpch.h"
#include "SocketMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

SocketMeshComponent::SocketMeshComponent(int jointIndex_, std::string meshFilename, std::string textureFilename)
	: MeshComponent(meshFilename, textureFilename)
{
	jointIndex = jointIndex_;
}

void SocketMeshComponent::Create()
{
	MeshComponent::Create();
}

void SocketMeshComponent::SetTransformFromLinkedSkeletonJoint()
{
	XMMATRIX jointMatrix = linkedSkeletalMesh->shaderSkinningData.skinningMatrices[jointIndex];
	transform.Decompose(jointMatrix * linkedSkeletalMesh->GetWorldMatrix());
	UpdateTransform();
}
