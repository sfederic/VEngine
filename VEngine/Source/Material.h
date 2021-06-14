#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Material
{
public:
	XMFLOAT4 colour;
};

class MaterialSystem
{
public:
	Material* CreateMaterial();
};
