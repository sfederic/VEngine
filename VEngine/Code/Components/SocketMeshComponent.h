#pragma once

#include "Components/MeshComponent.h"
#include "Animation/Joint.h"

class SkeletalMeshComponent;

//@Todo: need to figure out how to handle deleting SocketMeshes that are linked to SkeletalMeshes.
//If they're on the same actor, it's fine. But if they're separate from each other, socket won't delete.
//Also if the SkeletalMesh is deleted before the SocketMesh, the transform code will explode.

//Mesh that attaches to a Skeleton Joint. Updates its own transform using the Joint its attached to.
class SocketMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(SocketMeshComponent);

	SocketMeshComponent() {}
	SocketMeshComponent(int jointIndex_, std::string meshFilename, std::string textureFilename);

	void Create() override;

	void LinkToSkeletalMeshComponent(SkeletalMeshComponent* skeletalMesh) { linkedSkeletalMesh = skeletalMesh; }
	void SetTransformFromLinkedSkeletonJoint();

private:
	SkeletalMeshComponent* linkedSkeletalMesh = nullptr;

	Joint::JointIndex jointIndex = Joint::INVALID_JOINT_INDEX;
};
