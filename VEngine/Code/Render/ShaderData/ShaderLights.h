#pragma once

#include "Render/Light.h"

struct ShaderLights
{
	static const int MAX_LIGHTS = 32;

	DirectX::XMFLOAT4 eyePosition = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.0);
	DirectX::XMFLOAT4 globalAmbient = DirectX::XMFLOAT4(0.33f, 0.33f, 0.33f, 1.f);
	int numLights = 0;

	//Shadows for now are based on 1 directional light in the level. Set this to false to avoid the 
	//shadow map from building from an identity matrix when no directional lights exist in level.
	bool shadowsEnabled = false;

	int pad[2] = {};

	Light lights[MAX_LIGHTS] = {};
};
