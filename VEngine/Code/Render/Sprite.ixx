export module Render.Sprite;

import <DirectXMath.h>;

import Core.Transform;
import Render.VRect;
import <string>;

export struct Sprite
{
	Transform transform;

	std::string textureFilename;

	//Source Rect is the size of the texture to itself (note that you could render half a texture for example).
	VRect srcRect;

	//Destination Rect is the pixel dimensions and positions to render the texture at.
	VRect dstRect;

	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);

	float angle = 0.f;
	float z = 0.f;

	//this bool denotes whether the srcRect is used per a user definition or
	//whether it's values are made from the texture's width and height.
	bool useSourceRect = false;
};
