#include "SpriteBatcher.h"
#include "Render/Renderer.h"
#include "Render/RenderUtils.h"
#include "Render/TextureSystem.h"

SpriteBatcher spriteBatcher;

void SpriteBatcher::Init()
{
	spriteVertexBuffer = RenderUtils::CreateDynamicBuffer(4 * sizeof(Vertex),
		D3D11_BIND_VERTEX_BUFFER, &verts[0]);

	//Always a quad, for now
	MeshData::indexDataType spriteIndices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	spriteIndexBuffer = RenderUtils::CreateDefaultBuffer(6 * sizeof(MeshData::indexDataType),
		D3D11_BIND_INDEX_BUFFER, &spriteIndices[0]);
}

void SpriteBatcher::Reset()
{
	screenSprites.clear();
}

void SpriteBatcher::CreateScreenSprite(Sprite sprite)
{
	screenSprites.push_back(sprite);
}

XMFLOAT3 SpriteBatcher::PointToNdc(int x, int y, float z)
{
	XMFLOAT3 p;
	p.x = 2.0f * (float)x / renderer.GetViewportWidth() -1.0f;
	p.y = 1.0f - 2.0f * (float)y / renderer.GetViewportHeight();
	p.z = z;
	return p;
}

void SpriteBatcher::BuildSpriteQuadForViewportRendering(const Sprite& sprite)
{
	const D3D11_RECT& dst = sprite.dstRect;
	const D3D11_RECT& src = sprite.srcRect;

	// Dest rect defines target in screen space.
	verts[0].pos = PointToNdc(dst.left, dst.bottom, sprite.z);
	verts[1].pos = PointToNdc(dst.left, dst.top, sprite.z);
	verts[2].pos = PointToNdc(dst.right, dst.top, sprite.z);
	verts[3].pos = PointToNdc(dst.right, dst.bottom, sprite.z);

	Texture2D* texture = textureSystem.FindTexture2D(sprite.textureFilename);
	int texWidth = texture->desc.Width;
	int texHeight = texture->desc.Height;

	// Source rect defines subset of texture to use from sprite sheet.
	verts[0].uv = XMFLOAT2((float)src.left / texWidth, (float)src.bottom / texHeight);
	verts[1].uv = XMFLOAT2((float)src.left / texWidth, (float)src.top / texHeight);
	verts[2].uv = XMFLOAT2((float)src.right / texWidth, (float)src.top / texHeight);
	verts[3].uv = XMFLOAT2((float)src.right / texWidth, (float)src.bottom / texHeight);
	
	// Quad center point.
	float tx = 0.5f * (verts[0].pos.x + verts[3].pos.x);
	float ty = 0.5f * (verts[0].pos.y + verts[1].pos.y);

	XMVECTOR scaling = XMLoadFloat3(&sprite.transform.scale);
	XMVECTOR origin = XMVectorSet(tx, ty, 0.0f, 0.0f);
	XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX T = XMMatrixAffineTransformation2D(scaling, origin, sprite.angle, translation);

	// Rotate and scale the quad in NDC space.
	for (int i = 0; i < 4; i++)
	{
		XMVECTOR p = XMLoadFloat3(&verts[i].pos);
		p = XMVector3TransformCoord(p, T);
		XMStoreFloat3(&verts[i].pos, p);
	}
}

void SpriteBatcher::BuildSpriteQuadForParticleRendering()
{
	verts[0].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	verts[1].pos = XMFLOAT3(-1.f, 1.f, 0.f);
	verts[2].pos = XMFLOAT3(1.f, 1.f, 0.f);
	verts[3].pos = XMFLOAT3(1.f, -1.f, 0.f);

	verts[0].uv = XMFLOAT2(0.f, 0.f);
	verts[1].uv = XMFLOAT2(0.f, 1.f);
	verts[2].uv = XMFLOAT2(1.f, 1.f);
	verts[3].uv = XMFLOAT2(1.f, 0.f);
}
