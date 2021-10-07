#pragma once
#include <DirectXMath.h>
#include "Transform.h"

using namespace DirectX;

//REF:http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

struct Sprite
{
	Transform transform;

	D3D11_RECT srcRect;
	D3D11_RECT dstRect;

	float angle;
	float z = 0.2f;
};

struct SpriteBatch
{
	float texWidth = 128.f;
	float texHeight = 128.f;

	XMFLOAT3 PointToNdc(int x, int y, float z);
	void BuildSpriteQuad(const Sprite& sprite, Vertex v[4]);
};
