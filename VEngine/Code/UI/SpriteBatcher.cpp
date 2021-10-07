#include "SpriteBatcher.h"
#include "Render/Renderer.h"
#include "Render/RenderUtils.h"

SpriteBatcher::SpriteBatcher()
{
	sprite.dstRect = { 0, 0, 50, 50 };
	sprite.srcRect = { 0, 0, 512, 512 };
	BuildSpriteQuad(sprite, verts);
	spriteVertexBuffer = RenderUtils::CreateDefaultBuffer(4 * sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER, &verts[0]);

	//Always a quad for now
	uint32_t spriteIndices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	spriteIndexBuffer = RenderUtils::CreateDefaultBuffer(6 * sizeof(uint32_t), D3D11_BIND_INDEX_BUFFER, &spriteIndices[0]);
}

XMFLOAT3 SpriteBatcher::PointToNdc(int x, int y, float z)
{
	XMFLOAT3 p;
	p.x = 2.0f * (float)x / renderer.GetViewportWidth() -1.0f;
	p.y = 1.0f - 2.0f * (float)y / renderer.GetViewportHeight();
	p.z = z;
	return p;
}

void SpriteBatcher::BuildSpriteQuad(const Sprite& sprite, Vertex v[4])
{
	const D3D11_RECT& dst = sprite.dstRect;

	const D3D11_RECT& src = sprite.srcRect;

	// Dest rect defines target in screen space.
	v[0].pos = PointToNdc(dst.left, dst.bottom, sprite.z);
	v[1].pos = PointToNdc(dst.left, dst.top, sprite.z);
	v[2].pos = PointToNdc(dst.right, dst.top, sprite.z);
	v[3].pos = PointToNdc(dst.right, dst.bottom, sprite.z);

	// Source rect defines subset of texture to use from sprite sheet.
	v[0].uv = XMFLOAT2((float)src.left / texWidth, (float)src.bottom / texHeight);
	v[1].uv = XMFLOAT2((float)src.left / texWidth, (float)src.top / texHeight);
	v[2].uv = XMFLOAT2((float)src.right / texWidth, (float)src.top / texHeight);
	v[3].uv = XMFLOAT2((float)src.right / texWidth, (float)src.bottom / texHeight);

	// Quad center point.
	float tx = 0.5f * (v[0].pos.x + v[3].pos.x);
	float ty = 0.5f * (v[0].pos.y + v[1].pos.y);

	XMVECTOR scaling = XMLoadFloat3(&sprite.transform.scale);
	XMVECTOR origin = XMVectorSet(tx, ty, 0.0f, 0.0f);
	XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMMATRIX T = XMMatrixAffineTransformation2D(scaling, origin, sprite.angle, translation);

	// Rotate and scale the quad in NDC space.
	for (int i = 0; i < 4; ++i)
	{
		XMVECTOR p = XMLoadFloat3(&v[i].pos);
		p = XMVector3TransformCoord(p, T);
		XMStoreFloat3(&v[i].pos, p);
	}
}
