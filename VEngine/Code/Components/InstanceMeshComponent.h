#pragma once
#include "Components/MeshComponent.h"

struct InstanceMeshComponent : MeshComponent
{
	COMPONENT_SYSTEM(InstanceMeshComponent)

	uint32_t meshInstanceRenderCount = 0;

	InstanceMeshComponent(const char* filename, const wchar_t* shader = L"InstanceShader.hlsl");
	virtual void Create();
};
