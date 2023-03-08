#pragma once

#include <string>

class MeshComponent;

struct MeshComponentData
{
	std::string filename;
	MeshComponent* meshComponent = nullptr;
};

struct TextureData
{
	std::string filename;
};

struct ShaderData
{
	std::string shaderItemName;
};
