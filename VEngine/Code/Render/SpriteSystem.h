#pragma once
#include "System.h"
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "Transform.h"
#include "RenderTypes.h"

using namespace DirectX;

struct SpriteSheetEmitter;
struct ID3D11Buffer;
struct ID3D11DeviceContext;

//Sprite and text rendering for D3D11
//REF:http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

struct Sprite
{
	Transform transform;

	VRect srcRect;
	VRect dstRect;

	std::string textureFilename;

	float angle = 0.f;
	float z = 0.f;

	//this bool denotes whether the srcRect is used per a user definition or
	//whether it's values are made from the texture's width and height.
	bool useSourceRect = false;
};

//SpriteSystem is used for rendering both widget and particle images (screen space vs world space).
//Ideally you would seperate on-screen image and particle rendering out, but the quad building and buffers
//are generic enough to be shared between the two.
struct SpriteSystem : System
{
	ID3D11Buffer* spriteVertexBuffer = nullptr;
	ID3D11Buffer* spriteIndexBuffer = nullptr;

	Vertex verts[4] = {};

	std::vector<Sprite> screenSprites;

	SpriteSystem();
	void Init();
	void Reset();
	void CreateScreenSprite(Sprite sprite);
	void BuildSpriteQuadForViewportRendering(const Sprite& sprite);
	void BuildSpriteQuadForSpriteSheetRendering(const Sprite& sprite);
	void BuildSpriteQuadForParticleRendering();
	void UpdateAndSetSpriteBuffers(ID3D11DeviceContext* context);

private:
	XMFLOAT3 PointToNdc(int x, int y, float z);
};

extern SpriteSystem spriteSystem;
