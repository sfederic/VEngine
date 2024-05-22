#pragma once

#include "Render/LightData.h"

struct ShaderLights
{
	static const int MAX_LIGHTS = 32;

	DirectX::XMFLOAT4 globalAmbient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.f);
	int numLights = 0;

	//Shadows for now are based on 1 directional light in the level. Set this to false to avoid the 
	//shadow map from building from an identity matrix when no directional lights exist in level.
	bool shadowsEnabled = false;

	int pad[2] = {};

	LightData lights[MAX_LIGHTS] = {};
};
