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
	std::string textureFilename;

	Material* material = nullptr;
	MeshDataProxy* data = nullptr;
	PipelineStateObject* pso = nullptr;

	MeshComponent(const std::string filename_, const std::string textureFilename_);
	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps();
};
