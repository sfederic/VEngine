#pragma once
#include <string>
#include <vector>
#include "Component.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Transform.h"

struct MeshComponent : Component
{
	COMPONENT_SYSTEM(MeshComponent)

	std::string filename;
	MeshData data;
	PipelineStateObject pso;
	Transform transform;

	virtual void Create() override;
};
