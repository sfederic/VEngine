#pragma once
#include "System.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include "Transform.h"
#include "Render/RenderTypes.h"

using namespace DirectX;

//Sprite and text rendering for D3D11
//REF:http://www.d3dcoder.net/Data/Resources/SpritesAndText.pdf

//Sprites are images that are rendered on the viewport in screen space.
struct Sprite
{
	Transform transform;

	D3D11_RECT srcRect;
	D3D11_RECT dstRect;

	std::string textureFilename;

	float angle = 0.f;
	float z = 0.f;
};

//SpriteSystem is used for rendering both widget and particle images (screen space vs world space).
//Ideally you would seperate on-screen image and particle rendering out, but the quad building and buffers
//are generic enough to be shared between the two.
struct SpriteSystem : System
{
	ID3D11Buffer* spriteVertexBuffer = nullptr;
	ID3D11Buffer* spriteIndexBuffer = nullptr;

	//TODO: The same 4 vertices are used for every quad, can probably do some instancing here later.
	Vertex verts[4] = {};

	std::vector<Sprite> screenSprites;

	SpriteSystem();
	void Init();
	void Reset();
	void CreateScreenSprite(Sprite sprite);
	void BuildSpriteQuadForViewportRendering(const Sprite& sprite);
	void BuildSpriteQuadForParticleRendering();
	void UpdateAndSetSpriteBuffers(ID3D11DeviceContext* context);

private:
	XMFLOAT3 PointToNdc(int x, int y, float z);
};

extern SpriteSystem spriteSystem;
