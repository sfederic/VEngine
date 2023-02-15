#include "vpch.h"
#include "SocketMeshComponent.h"

SocketMeshComponent::SocketMeshComponent(int jointIndex_, std::string meshFilename, std::string textureFilename)
{
	MeshComponent(meshFilename, textureFilename);
	jointIndex = jointIndex_;
}

void SocketMeshComponent::Create()
{
	MeshComponent::Create();
}
