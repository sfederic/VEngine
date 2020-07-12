#include "Actor.h"
#include "DXUtil.h"
#include "Array.h"

Actor::Actor()
{
	transform = XMMatrixIdentity();
}

XMVECTOR Actor::GetPositionVector()
{
	return transform.r[3];
}

XMFLOAT3 Actor::GetPositionFloat3()
{
	XMVECTOR pos = transform.r[3];
	return XMFLOAT3(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2]);
}

void Actor::SetPosition(XMVECTOR v)
{
	transform = XMMatrixTranslationFromVector(v);
}

void Actor::SetPosition(float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x, y, z, 1.0f);
	transform = XMMatrixTranslationFromVector(v);
}

void Actor::SetRotation(XMVECTOR axis, float angle)
{
	transform = XMMatrixRotationAxis(axis, angle);
}

XMMATRIX Actor::GetRotation()
{
	return transform;
}

XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(transform.r[0].m128_f32[0], transform.r[1].m128_f32[1], transform.r[2].m128_f32[2]);
}

ActorSystem::ActorSystem()
{
}

void ActorSystem::CreateActors(const char* modelFilename, DXUtil* dx, int numActorsToSpawn)
{
	if (loadOBJFile(modelFilename, modelData))
	{
		UINT byteWidth = modelData.GetByteWidth();
		numVertices = (byteWidth * actors.size()) / sizeof(Vertex);
		dx->CreateVertexBuffer(byteWidth, modelData.verts.data(), this);

		size_t stride = sizeof(Vertex);
		//TODO: dislay debug rendering for sphere

		BoundingBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
		BoundingSphere::CreateFromBoundingBox(boundingSphere, boundingBox);

		//actors.reserve(numActorsToSpawn);
		for (int i = 0; i < numActorsToSpawn; i++)
		{
			Actor actor;
			actor.transform.r[3] = XMVectorSet(i, i, i, 1.f);
			actor.vertexBufferOffset = i * modelData.GetByteWidth();
			actor.boundingBox = boundingBox;
			actor.boundingSphere = boundingSphere;
			//actors.push_back(actor);
			actors.push_back(actor);
		}


	}
	else
	{
		OutputDebugString("Actor failed to load");
	}
}