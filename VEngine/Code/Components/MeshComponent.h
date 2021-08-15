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

	inline static std::string createFilename;

	std::string filename;
	MeshData data;
	PipelineStateObject pso;

	virtual void Create() override;
};
