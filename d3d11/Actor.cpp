#include "Actor.h"
#include "RenderSystem.h"
#include "Array.h"
#include "Debug.h"
#include "Obj.h"
#include "FBXImporter.h"

//CONSTRUCTORS
Actor::Actor()
{
	transform = XMMatrixIdentity();
}

//POSITION FUNCTIONS
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

//ROTATION FUNCTIONS
void Actor::SetRotation(XMVECTOR axis, float angle)
{
	transform = XMMatrixRotationAxis(axis, angle);
}

XMMATRIX Actor::GetRotation()
{
	return transform;
}

//SCALE FUNCTIONS
XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(transform.r[0].m128_f32[0], transform.r[1].m128_f32[1], transform.r[2].m128_f32[2]);
}

void Actor::SetScale(float x, float y, float z)
{
	transform = XMMatrixScaling(x, y, z);
}

void Actor::SetScale(XMVECTOR scale)
{
	transform = XMMatrixScalingFromVector(scale);
}

void Actor::SetScale(XMFLOAT3 scale)
{
	XMVECTOR scaleVec = XMLoadFloat3(&scale);
	transform = XMMatrixScalingFromVector(scaleVec);
}

//VECTOR FUNCTIONS
XMVECTOR Actor::GetForwardVector()
{
	return transform.r[2];
}

XMVECTOR Actor::GetRightVector()
{
	return transform.r[0];
}

XMVECTOR Actor::GetUpVector()
{
	return transform.r[1];
}

void Actor::Move(float d, XMVECTOR direction)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR loc = GetPositionVector();
	loc += XMVectorMultiplyAdd(s, direction, loc);
	SetPosition(loc);
}

//ACTOR SYSTEM
void ActorSystem::CreateActors(RenderSystem* dx, int numActorsToSpawn)
{
	char filename[128] = {};
	strcat_s(filename, "Models/");
	strcat_s(filename, modelName);

	//if (LoadOBJFile(filename, modelData))
	if (FBXImporter::Import(filename, modelData))
	{
		UINT byteWidth = modelData.GetByteWidth();
		numVertices = (byteWidth * actors.size()) / sizeof(Vertex);
		dx->CreateVertexBuffer(byteWidth, modelData.verts.data(), this);
		UINT indicesByteWidth = modelData.indices.size() * sizeof(uint16_t);
		indexBuffer = dx->CreateDefaultBuffer(indicesByteWidth, D3D11_BIND_INDEX_BUFFER, modelData.indices.data());

		dx->CreateSamplerState(this);
		dx->CreateTexture(this);

		size_t stride = sizeof(Vertex);

		BoundingBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
		BoundingSphere::CreateFromPoints(boundingSphere, modelData.verts.size(), &modelData.verts[0].pos, stride);

		actors.reserve(numActorsToSpawn);
		for (int i = 0; i < numActorsToSpawn; i++)
		{
			//TODO: I'm gonna need the smart pointers here to deal with the eventual Tick() virtual calls
			Actor actor;
			actor.transform.r[3] = XMVectorSet(i, i, i, 1.f);
			actor.vertexBufferOffset = i * modelData.GetByteWidth();

			actors.push_back(actor);
		}
	}
	else
	{
		DebugPrint("Actors failed to load");
	}
}

void ActorSystem::AddActor()
{
	Actor actor = Actor();
	actor.vertexBufferOffset = (int)(actors.size() * modelData.GetByteWidth());

	actors.push_back(actor);
}

void ActorSystem::RemoveActor(int index)
{
	actors.erase(actors.begin() + index);
}
