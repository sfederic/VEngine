#pragma once
#include "Components/MeshComponent.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

struct InstanceMeshComponent : MeshComponent
{
	COMPONENT_SYSTEM(InstanceMeshComponent)

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	std::vector<InstanceData> instanceData;

private:
	uint32_t meshInstanceRenderCount = 0;

public:
	InstanceMeshComponent(
		uint32_t meshInstanceRenderCount_, const char* filename, const wchar_t* shader = L"InstanceShader.hlsl");
	~InstanceMeshComponent();
	virtual void Create();
};
