#pragma once

#include <string>
#include "Transform.h"
#include "RenderTypes.h"

struct Sprite
{
	Transform transform;

	VRect srcRect = {};
	VRect dstRect = {};

	std::string textureFilename;

	float angle = 0.f;
	float z = 0.f;

	//this bool denotes whether the srcRect is used per a user definition or
	//whether it's values are made from the texture's width and height.
	bool useSourceRect = false;
};
