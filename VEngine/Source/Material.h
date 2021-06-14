#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>

using namespace DirectX;

class Material
{
public:
	std::string name;
	XMFLOAT4 colour;
};

class MaterialSystem
{
public:
	Material* CreateMaterialFromFile(const std::string& filename);

	std::vector<Material> materials;
};
