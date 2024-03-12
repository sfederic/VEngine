#include "vpch.h"
#include "Polyboard.h"
#include "Core/Camera.h"
#include "Render/RenderUtils.h"
#include "Physics/Raycast.h"

Polyboard::Polyboard()
{
	startPoint = XMFLOAT3(0.f, 0.f, 0.f);
	endPoint = XMFLOAT3(0.f, 0.f, 0.f);

	textureData.filename = "Particle/beam_blue.png";
}

void Polyboard::Create()
{
	GenerateVertices();
}

void Polyboard::Tick(float deltaTime)
{
	if (setToDestroy)
	{
		destroyTimer += deltaTime;
		if (destroyTimer > destroyTimerMax)
		{
			Remove();
		}
	}
}

Properties Polyboard::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Polyboard::GenerateVertices()
{
	CalcVertices();

	if (vertices.size() == 0)
	{
		vertices.emplace_back(Vertex());
	}

	constexpr uint32_t cappedBufferSize = 512;
	vertexBuffer.CreateDynamicCapped(vertices, cappedBufferSize);
	indexBuffer.CreateDynamicCapped(indices, cappedBufferSize);
}

void Polyboard::CalcVertices()
{
	vertices.clear();
	indices.clear();

	const XMVECTOR start = XMLoadFloat3(&startPoint);
	const XMVECTOR end = XMLoadFloat3(&endPoint);
	const XMVECTOR tangent = XMVector3Normalize(end - start);

	//@Todo: it's hard to get a nice increment here to add vertices to. For now it looks rough
	//because the PolyBoard vertices are generated based on distance to the end point.
	float dist = XMVector3Length(end - start).m128_f32[0];
	dist = std::ceil(dist);

	const auto camForward = Camera::GetActiveCamera().GetForwardVectorV();

	const float incr = dist / 10.f;

	for (float i = 0; i < dist; i += incr)
	{
		const XMVECTOR pos = start + (tangent * i);

		//Use this if you need a wavy effect for polyboards
		//radius = (sinf(i * Core::timeSinceStartup) * 0.15f) + 0.4f;

		const XMVECTOR p1 = pos - (radius * XMVector3Cross(tangent, camForward));
		const XMVECTOR p2 = pos + (radius * XMVector3Cross(tangent, camForward));

		Vertex vertex1 = {}, vertex2 = {};

		DirectX::XMStoreFloat3(&vertex1.pos, p1);
		DirectX::XMStoreFloat3(&vertex2.pos, p2);

		vertex1.uv = XMFLOAT2(0.f, i);
		vertex2.uv = XMFLOAT2(1.f, i);

		vertices.emplace_back(vertex1);
		vertices.emplace_back(vertex2);
	}

	const XMVECTOR p1 = end - (radius * XMVector3Cross(tangent, camForward));
	const XMVECTOR p2 = end + (radius * XMVector3Cross(tangent, camForward));

	Vertex vertex1 = {}, vertex2 = {};

	DirectX::XMStoreFloat3(&vertex1.pos, p1);
	DirectX::XMStoreFloat3(&vertex2.pos, p2);

	vertex1.uv = XMFLOAT2(0.f, 0);
	vertex2.uv = XMFLOAT2(1.f, 0);

	vertices.emplace_back(vertex1);
	vertices.emplace_back(vertex2);

	//Because the vertices aren't triangle based, divide by 3
	for (int i = 0; i < (vertices.size() / 3) + 1; i++)
	{
		indices.emplace_back(0 + (2 * i));
		indices.emplace_back(2 + (2 * i));
		indices.emplace_back(1 + (2 * i));

		indices.emplace_back(2 + (2 * i));
		indices.emplace_back(3 + (2 * i));
		indices.emplace_back(1 + (2 * i));
	}
}

void Polyboard::SetStartPoint(const XMVECTOR start)
{
	DirectX::XMStoreFloat3(&startPoint, start);
}

void Polyboard::SetEndPoint(const XMVECTOR end)
{
	DirectX::XMStoreFloat3(&endPoint, end);
}

bool Polyboard::RaycastFromStartToEndPoints(HitResult& hit)
{
	return Raycast(hit, XMLoadFloat3(&startPoint), XMLoadFloat3(&endPoint));
}

void Polyboard::SetDestroyTimer(float timerMax)
{
	setToDestroy = true;
	destroyTimerMax = timerMax;
}
