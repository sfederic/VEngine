#pragma once
#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Transform.h"

struct MeshComponent : SpatialComponent
{
	COMPONENT_SYSTEM(MeshComponent)

	std::string filename;
	MeshDataProxy* data = nullptr;
	PipelineStateObject* pso = nullptr;

	MeshComponent(const char* filename_);
	virtual void Create() override;
	virtual Properties GetProperties();
};
