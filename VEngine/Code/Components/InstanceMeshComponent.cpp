#include "InstanceMeshComponent.h"

InstanceMeshComponent::InstanceMeshComponent(const char* filename, const wchar_t* shader)
	: MeshComponent(filename, shader)
{
}

void InstanceMeshComponent::Create()
{
	MeshComponent::Create();
}
