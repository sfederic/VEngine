#pragma once
#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Transform.h"

struct ShaderItem;
struct Material;

struct MeshComponent : SpatialComponent
{
	COMPONENT_SYSTEM(MeshComponent)

	std::string meshFilename;
	std::wstring textureFilename;

	Material* material = nullptr;
	MeshDataProxy* data = nullptr;
	PipelineStateObject* pso = nullptr;

	MeshComponent(const char* filename_, const wchar_t* textureFilename_);
	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps();
};
