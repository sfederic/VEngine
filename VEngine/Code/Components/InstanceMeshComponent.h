#pragma once

#include "Components/MeshComponent.h"
#include "Render/ShaderData/InstanceData.h"

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//InstanceMeshComponent doesn't have the individual meshes moved around in editor right now.
class InstanceMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(InstanceMeshComponent);

	ID3D11Buffer* structuredBuffer = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	InstanceMeshComponent() {}
	InstanceMeshComponent(uint32_t meshInstanceRenderCount_,
		const std::string filename,
		const std::string textureFilename,
		ShaderItem* shaderItem);
	~InstanceMeshComponent();

	void Create() override;
	Properties GetProps() override;

	void SetInstanceCount(uint32_t count);
	uint32_t GetInstanceCount();

	void SetInstanceData(std::vector<InstanceData>& instanceData_);
	auto& GetInstanceData() { return instanceData; }

	void ReleaseBuffers();

private:
	std::vector<InstanceData> instanceData;

	uint32_t meshInstanceRenderCount = 0;
};
