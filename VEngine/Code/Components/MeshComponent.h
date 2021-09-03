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
	std::wstring textureFilename;

	MeshDataProxy* data = nullptr;
	PipelineStateObject* pso = nullptr;
	ShaderItem* shader = nullptr;
	int number;

	MeshComponent(const char* filename_, std::wstring textureName = L"", const wchar_t* shader = L"DefaultShader.hlsl");
	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps();
};
