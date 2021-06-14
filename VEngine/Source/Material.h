#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Properties.h"

using namespace DirectX;

class Material
{
public:
	Properties GetProps();
	void SaveToFile();

	std::string name;
	XMFLOAT4 colour;
};

class MaterialSystem
{
public:
	Material* CreateMaterialFromFile(const std::string& filename);

	std::vector<Material> materials;
};
