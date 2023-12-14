#pragma once

#include <string>
#include "Core/Transform.h"
#include "VRect.h"

struct Sprite
{
	Transform transform;

	//Source Rect is the size of the texture to itself (note that you could render half a texture for example).
	VRect srcRect;

	//Destination Rect is the pixel dimensions and positions to render the texture at.
	VRect dstRect;

	std::string textureFilename;

	float angle = 0.f;
	float z = 0.f;

	//this bool denotes whether the srcRect is used per a user definition or
	//whether it's values are made from the texture's width and height.
	bool useSourceRect = false;
};
