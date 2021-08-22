#pragma once
#include <string>
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include "Render/RenderTypes.h"
#include "Render/PipelineObjects.h"
#include "Transform.h"

struct ShaderItem;

struct MeshComponent : SpatialComponent
{
	COMPONENT_SYSTEM(MeshComponent)

	std::string filename;
	MeshDataProxy* data = nullptr;
	PipelineStateObject* pso = nullptr;
	ShaderItem* shader = nullptr;

	MeshComponent(const char* filename_, const wchar_t* shader = L"DefaultShader.hlsl");
	virtual void Create() override;
	virtual Properties GetProps();
};
