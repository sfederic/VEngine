#pragma once
#include "Components/MeshComponent.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//InstanceMeshComponent doesn't have the individual meshes moved around in editor right now.
struct InstanceMeshComponent : MeshComponent
{
	COMPONENT_SYSTEM(InstanceMeshComponent)

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	std::vector<InstanceData> instanceData;

private:
	uint32_t meshInstanceRenderCount = 0;

public:
	InstanceMeshComponent(uint32_t meshInstanceRenderCount_,
		const std::string filename, 
		const std::string textureFilename);
	~InstanceMeshComponent();
	virtual void Create();
	void SetInstanceCount(uint32_t count);
	uint32_t GetInstanceCount();
	void SetInstanceData(std::vector<InstanceData>& instanceData_);
	virtual Properties GetProps();
	void ReleaseBuffers();
};
