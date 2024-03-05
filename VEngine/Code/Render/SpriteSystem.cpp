#include "vpch.h"
#include "SpriteSystem.h"
#include "Renderer.h"
#include "RenderUtils.h"
#include "TextureSystem.h"
#include "Core/Debug.h"
#include "Render/Vertex.h"
#include "Render/MeshData.h"
#include "Render/Texture2D.h"

static SystemStates systemState = SystemStates::Unloaded;
static Microsoft::WRL::ComPtr<ID3D11Buffer> spriteVertexBuffer;
static Microsoft::WRL::ComPtr<ID3D11Buffer> spriteIndexBuffer;
static Vertex verts[4];
static std::vector<Sprite> screenSprites;

XMFLOAT3 PointToNdc(int x, int y, float z);

void SpriteSystem::Init()
{
	RenderUtils::CreateDynamicBuffer(4 * sizeof(Vertex),
		D3D11_BIND_VERTEX_BUFFER, &verts[0], spriteVertexBuffer);

	//Always a quad, for now
	MeshData::indexDataType spriteIndices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	RenderUtils::CreateDefaultBuffer(6 * sizeof(MeshData::indexDataType),
		D3D11_BIND_INDEX_BUFFER, &spriteIndices[0], spriteIndexBuffer);
}

void SpriteSystem::Reset()
{
	//in-game UI sprites (not particle) need to be cleared out at the end of the frame
	//because of the imgui-esque way widgets render their content in-game.
	screenSprites.clear();
}

void SpriteSystem::CreateScreenSprite(Sprite& sprite)
{
	screenSprites.emplace_back(sprite);
}

void SpriteSystem::UpdateAndSetSpriteBuffers()
{
	ID3D11DeviceContext& context = Renderer::GetDeviceContext();

	//Update vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HR(context.Map(spriteVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, &verts, sizeof(verts));
	context.Unmap(spriteVertexBuffer.Get(), 0);

	context.IASetVertexBuffers(0, 1, spriteVertexBuffer.GetAddressOf(), &Renderer::stride, &Renderer::offset);
	context.IASetIndexBuffer(spriteIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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

	const auto texture = TextureSystem::FindTexture2D(sprite.textureFilename);
	const auto texWidth = texture->GetWidth();
	const auto texHeight = texture->GetHeight();

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
	const float tx = 0.5f * (verts[0].pos.x + verts[3].pos.x);
	const float ty = 0.5f * (verts[0].pos.y + verts[1].pos.y);

	const XMVECTOR scaling = XMLoadFloat3(&sprite.transform.scale);
	const XMVECTOR origin = XMVectorSet(tx, ty, 0.0f, 0.0f);
	const XMVECTOR translation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	const XMMATRIX T = XMMatrixAffineTransformation2D(scaling, origin, sprite.angle, translation);

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
