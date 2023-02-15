#pragma once

#include "Components/MeshComponent.h"
#include "Animation/AnimationValues.h"

//Mesh that attaches to a Skeleton Joint. Updates its own transform using the Joint its attached to.
class SocketMeshComponent : public MeshComponent
{
public:
	SocketMeshComponent(int jointIndex_, std::string meshFilename, std::string textureFilename);

	void Create() override;

private:
	int jointIndex = INVALID_JOINT_INDEX;
};
