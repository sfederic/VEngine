#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include "Transform.h"
#include "Render/RenderTypes.h"

using namespace DirectX;

//Sprite and text rendering for D3D11
//REF:http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

struct Sprite
{
	Transform transform;
	XMVECTOR velocity = XMVectorZero();

	D3D11_RECT srcRect;
	D3D11_RECT dstRect;

	std::string textureFilename;

	float angle = 0.f;
	float z = 0.f;

	float speed = 5.0f;
	float lifetime = 1.0f;
};

struct SpriteBatcher
{
	ID3D11Buffer* spriteVertexBuffer = nullptr;
	ID3D11Buffer* spriteIndexBuffer = nullptr;

	//TODO: The same 4 vertices are used for every quad, can probably do some instancing here later.
	Vertex verts[4];

	//Sprites rendering for UI
	std::vector<Sprite> screenSprites;

	//Sprites rendering in-world (eg. particles)
	std::vector<Sprite> worldSprites;

	void Init();
	void Tick(float deltaTime);
	void Reset();
	void CreateWorldSprite(Sprite sprite);
	void CreateScreenSprite(Sprite sprite);
	void BuildSpriteQuadForViewportRendering(const Sprite& sprite);
	void BuildSpriteQuadForParticleRendering();

private:
	XMFLOAT3 PointToNdc(int x, int y, float z);
};

extern SpriteBatcher spriteBatcher;
