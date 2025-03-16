#pragma once

import Components.MeshComponent;
#include "Render/ShaderData/InstanceData.h"
#include <wrl.h>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

//InstanceMeshComponent doesn't have the individual meshes moved around in editor right now.
class InstanceMeshComponent : public MeshComponent
{
public:
	COMPONENT_SYSTEM(InstanceMeshComponent);

	Microsoft::WRL::ComPtr<ID3D11Buffer> structuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	InstanceMeshComponent() {}
	InstanceMeshComponent(uint32_t meshInstanceRenderCount_,
		const std::string filename,
		const std::string textureFilename,
		const std::string shaderItemName);

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
