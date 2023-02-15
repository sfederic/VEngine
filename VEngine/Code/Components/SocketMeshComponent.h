#pragma once

#include "Components/MeshComponent.h"
#include "Animation/AnimationValues.h"

class SkeletalMeshComponent;

//Mesh that attaches to a Skeleton Joint. Updates its own transform using the Joint its attached to.
class SocketMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(SocketMeshComponent);

	SocketMeshComponent() {}
	SocketMeshComponent(int jointIndex_, std::string meshFilename, std::string textureFilename);

	void Create() override;

	void LinkToSkeletalMeshComponent(SkeletalMeshComponent* skeletalMesh);
	void SetTransformFromLinkedSkeletonJoint();

private:
	SkeletalMeshComponent* linkedSkeletalMesh = nullptr;

	JointIndex jointIndex = INVALID_JOINT_INDEX;
};
