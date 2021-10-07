#pragma once
#include <DirectXMath.h>
//TODO: come back here and get rid of the <d3d11.h>
#include <d3d11.h>
#include "Transform.h"
#include "Render/RenderTypes.h"

using namespace DirectX;

//Sprite and text rendering for D3D11
//REF:http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

struct Sprite
{
	Transform transform;

	D3D11_RECT srcRect;
	D3D11_RECT dstRect;

	float angle = 0.f;
	float z = 0.f;
};

struct SpriteBatcher
{
	//TODO: come back here, pass the texture into BuildSpriteQuad(), grab the textire dimensions
	//through GetDesc() and set these two accordingly.
	int texWidth = 512;
	int texHeight = 512;

	ID3D11Buffer* spriteVertexBuffer = nullptr;
	ID3D11Buffer* spriteIndexBuffer = nullptr;
	Vertex verts[4];
	Sprite sprite;

	SpriteBatcher();
	XMFLOAT3 PointToNdc(int x, int y, float z);
	void BuildSpriteQuad(const Sprite& sprite, Vertex v[4]);
};
