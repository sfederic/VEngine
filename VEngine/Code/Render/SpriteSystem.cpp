#include "vpch.h"
#include "SpriteSystem.h"
#include "Renderer.h"
#include "RenderUtils.h"
#include "TextureSystem.h"
#include "Core/Debug.h"
#include "Particle/SpriteSheet.h"
#include "Texture2D.h"
#include "Render/Vertex.h"
#include "Render/MeshData.h"

static SystemStates systemState = SystemStates::Unloaded;
ID3D11Buffer* spriteVertexBuffer = nullptr;
ID3D11Buffer* spriteIndexBuffer = nullptr;
Vertex verts[4] = {};
std::vector<Sprite> screenSprites;

XMFLOAT3 PointToNdc(int x, int y, float z);

void SpriteSystem::Init()
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

void SpriteSystem::Reset()
{
	//in-game UI sprites (not particle) need to be cleared out at the end of the frame
	//because of the imgui-esque way widgets render their content in-game.
	screenSprites.clear();
}

void SpriteSystem::CreateScreenSprite(Sprite sprite)
{
	screenSprites.emplace_back(sprite);
}

void SpriteSystem::UpdateAndSetSpriteBuffers(ID3D11DeviceContext* context)
{
	//Update vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HR(context->Map(spriteVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, &verts, sizeof(verts));
	context->Unmap(spriteVertexBuffer, 0);

	context->IASetVertexBuffers(0, 1, &spriteVertexBuffer, &Renderer::stride, &Renderer::offset);
	context->IASetIndexBuffer(spriteIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

std::vector<Sprite>& SpriteSystem::GetScreenSprites()
{
	return screenSprites;
}

void SpriteSystem::BuildSpriteQuadForViewportRendering(const Sprite& sprite)
{
	VRect dst = sprite.dstRect;
	VRect src = sprite.srcRect;

	// Dest rect defines target in screen space.
	verts[0].pos = PointToNdc(dst.left, dst.bottom, sprite.z);
	verts[1].pos = PointToNdc(dst.left, dst.top, sprite.z);
	verts[2].pos = PointToNdc(dst.right, dst.top, sprite.z);
	verts[3].pos = PointToNdc(dst.right, dst.bottom, sprite.z);

	auto texture = TextureSystem::FindTexture2D(sprite.textureFilename);
	auto texWidth = texture->GetWidth();
	auto texHeight = texture->GetHeight();

	if (!sprite.useSourceRect)
	{
		src.right = texWidth;
		src.bottom = texHeight;
	}

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

void SpriteSystem::BuildSpriteQuadForSpriteSheetRendering(const Sprite& sprite)
{
	VRect src = sprite.srcRect;

	verts[0].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	verts[1].pos = XMFLOAT3(-1.f, 1.f, 0.f);
	verts[2].pos = XMFLOAT3(1.f, 1.f, 0.f);
	verts[3].pos = XMFLOAT3(1.f, -1.f, 0.f);

	verts[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[3].normal = XMFLOAT3(0.f, 0.f, -1.f);

	auto texture = TextureSystem::FindTexture2D(sprite.textureFilename);
	auto texWidth = texture->GetWidth();
	auto texHeight = texture->GetHeight();

	if (!sprite.useSourceRect)
	{
		src.right = texWidth;
		src.bottom = texHeight;
	}

	// Source rect defines subset of texture to use from sprite sheet.
	verts[0].uv = XMFLOAT2((float)src.left / texWidth, (float)src.bottom / texHeight);
	verts[1].uv = XMFLOAT2((float)src.left / texWidth, (float)src.top / texHeight);
	verts[2].uv = XMFLOAT2((float)src.right / texWidth, (float)src.top / texHeight);
	verts[3].uv = XMFLOAT2((float)src.right / texWidth, (float)src.bottom / texHeight);
}

void SpriteSystem::BuildSpriteQuadForParticleRendering()
{
	verts[0].pos = XMFLOAT3(-1.f, -1.f, 0.f);
	verts[1].pos = XMFLOAT3(-1.f, 1.f, 0.f);
	verts[2].pos = XMFLOAT3(1.f, 1.f, 0.f);
	verts[3].pos = XMFLOAT3(1.f, -1.f, 0.f);

	verts[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
	verts[3].normal = XMFLOAT3(0.f, 0.f, -1.f);

	verts[0].uv = XMFLOAT2(0.f, 0.f);
	verts[1].uv = XMFLOAT2(0.f, 1.f);
	verts[2].uv = XMFLOAT2(1.f, 1.f);
	verts[3].uv = XMFLOAT2(1.f, 0.f);
}

XMFLOAT3 PointToNdc(int x, int y, float z)
{
	XMFLOAT3 p = {};
	p.x = 2.0f * (float)x / Renderer::GetViewportWidth() - 1.0f;
	p.y = 1.0f - 2.0f * (float)y / Renderer::GetViewportHeight();
	p.z = z;
	return p;
}
