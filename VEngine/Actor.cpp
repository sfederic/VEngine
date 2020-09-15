#include "Actor.h"
#include "RenderSystem.h"
#include "Array.h"
#include "Debug.h"
#include "Obj.h"
#include "FBXImporter.h"
#include "MathHelpers.h"
#include "IRenderSystem.h"
#include "WICTextureLoader.h"

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

void Actor::SetPosition(XMFLOAT3 pos)
{
	transform.r[3] = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
}

//ROTATION FUNCTIONS
void Actor::SetRotation(XMVECTOR axis, float angle)
{
	transform *= XMMatrixRotationAxis(axis, angle);
}

void Actor::SetRotation(float roll, float pitch, float yaw)
{
	transform *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

void Actor::SetRotation(XMFLOAT3 rollPitchYaw)
{
	transform *= XMMatrixRotationRollPitchYaw(rollPitchYaw.x, rollPitchYaw.y, rollPitchYaw.z);
}

XMMATRIX Actor::GetRotation()
{
	return transform;
}

XMFLOAT3 Actor::GetRollPitchYaw()
{
	XMVECTOR vec = RollPitchYawFromMatrix(transform);
	XMFLOAT3 rot(vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2]);
	return rot;
}

//SCALE FUNCTIONS
XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(transform.r[0].m128_f32[0], transform.r[1].m128_f32[1], transform.r[2].m128_f32[2]);
}

void Actor::AddScale(float scale)
{

}

void Actor::AddScale(float x, float y, float z)
{

}

void Actor::AddScale(XMFLOAT3 scale)
{
	
}

void Actor::SetScale(float x, float y, float z)
{
	transform *= XMMatrixScaling(x, y, z);
}

void Actor::SetScale(XMVECTOR scale)
{
	transform *= XMMatrixScalingFromVector(scale);
}

void Actor::SetScale(XMFLOAT3 scale)
{
	XMVECTOR scaleVec = XMLoadFloat3(&scale);
	transform *= XMMatrixScalingFromVector(scaleVec);
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
	loc = XMVectorMultiplyAdd(s, direction, loc);
	SetPosition(loc);
}

//ACTOR SYSTEM
void ActorSystem::CreateActors(IRenderSystem* renderSystem, int numActorsToSpawn)
{
	char filename[128] = {};
	strcat_s(filename, "Models/");
	strcat_s(filename, modelName);

	//if (LoadOBJFile(filename, modelData))
	if (FBXImporter::Import(filename, modelData))
	{
		UINT byteWidth = modelData.GetByteWidth();
		numVertices = (byteWidth * actors.size()) / sizeof(Vertex);
		renderSystem->CreateVertexBuffer(byteWidth, modelData.verts.data(), this);
		UINT indicesByteWidth = modelData.indices.size() * sizeof(uint16_t);
		//indexBuffer = renderSystem->CreateDefaultBuffer(indicesByteWidth, D3D11_BIND_INDEX_BUFFER, modelData.indices.data());

		renderSystem->CreateSamplerState(this);
		renderSystem->CreateTexture(this);

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
			actor.name = name;
			std::wstring indexString = std::to_wstring(i);
			actor.name += indexString;

			actors.push_back(actor);
		}
	}
	else
	{
		DebugPrint("Actors failed to load");
	}
}

Actor* ActorSystem::AddActor(XMVECTOR spawnPosition)
{
	Actor actor = Actor();
	actor.SetPosition(spawnPosition);
	actor.vertexBufferOffset = (int)(actors.size() * modelData.GetByteWidth());

	actors.push_back(actor);
	return &actors[actors.size() - 1];
}

void ActorSystem::RemoveActor(int index)
{
	assert(index < actors.size());
	actors.erase(actors.begin() + index);
}

Actor* ActorSystem::GetActor(unsigned int index)
{
	assert(index < actors.size());
	return &actors[index];
}
