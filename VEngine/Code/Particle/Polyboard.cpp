#include "Polyboard.h"
#include "Camera.h"
#include "Core.h"
#include "Render/RenderUtils.h"

Polyboard::Polyboard()
{
	startPoint = XMFLOAT3(0.f, 0.f, 0.f);
	endPoint = XMFLOAT3(0.f, 0.f, 0.f);

	vertexBuffer = new Buffer();
	indexBuffer = new Buffer();

	textureData.filename = "beam_blue.png";
}

Properties Polyboard::GetProps()
{
	return Properties("Polyboard");
}

void Polyboard::GenerateVertices()
{
	CalcVertices();

	if (vertices.size() == 0)
	{
		vertices.push_back(Vertex());
	}

	vertexBuffer->data = RenderUtils::CreateDynamicBuffer(sizeof(Vertex) * 256,
		D3D11_BIND_VERTEX_BUFFER, vertices.data());

	indexBuffer->data = RenderUtils::CreateDynamicBuffer(256 * sizeof(MeshData::indexDataType),
		D3D11_BIND_INDEX_BUFFER, indices.data());
}

void Polyboard::CalcVertices()
{
	vertices.clear();
	indices.clear();

	XMVECTOR start = XMLoadFloat3(&startPoint);
	XMVECTOR end = XMLoadFloat3(&endPoint);
	XMVECTOR startToEndDir = XMVector3Normalize(end - start);

	float length = XMVector3Length(end - start).m128_f32[0];
	length = std::ceilf(length);

	XMVECTOR cameraPos = activeCamera->GetWorldPositionV();

	for (int i = 0; i < length; i++)
	{
		XMVECTOR pos = start + (startToEndDir * i);
		XMVECTOR nextPos = pos + startToEndDir;

		XMVECTOR posToCamera = XMVector3Normalize(pos - cameraPos);
		XMVECTOR tangent = XMVector3Normalize(nextPos - pos);

		//Use this if you need a wavy effect for polyboards
		radius = (sinf(i * Core::timeSinceStartup) * 0.15f) + 0.4f;

		XMVECTOR p1 = pos - (radius * XMVector3Cross(tangent, posToCamera));
		XMVECTOR p2 = pos + (radius * XMVector3Cross(tangent, posToCamera));

		Vertex vertex1 = {}, vertex2 = {};

		XMStoreFloat3(&vertex1.pos, p1);
		XMStoreFloat3(&vertex2.pos, p2);

		vertex1.uv = XMFLOAT2(0.f, i);
		vertex2.uv = XMFLOAT2(1.f, i);

		vertices.push_back(vertex1);
		vertices.push_back(vertex2);
	}

	for (size_t i = 0; i < (vertices.size() / 3) + 1; i++) //Because the vertices aren't triangle based, divide by 3
	{
		indices.push_back(0 + (2 * i));
		indices.push_back(2 + (2 * i));
		indices.push_back(1 + (2 * i));

		indices.push_back(2 + (2 * i));
		indices.push_back(3 + (2 * i));
		indices.push_back(1 + (2 * i));
	}
}
