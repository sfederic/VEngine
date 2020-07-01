#include "Actor.h"
#include "DXUtil.h"

Actor::Actor()
{
	transform = XMMatrixIdentity();
}

XMVECTOR Actor::GetActorPosition()
{
	return transform.r[3];
}

void Actor::SetActorPosition(XMVECTOR v)
{
	transform = XMMatrixTranslationFromVector(v);
}

void Actor::SetActorPosition(float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x, y, z, 1.0f);
	transform = XMMatrixTranslationFromVector(v);
}

void Actor::SetActorRotation(XMVECTOR axis, float angle)
{
	transform = XMMatrixRotationAxis(axis, angle);
}

XMMATRIX Actor::GetActorRotation()
{
	return transform;
}

void ActorSystem::CreateActors(const char* modelFilename, DXUtil* dx, int numActorsToSpawn)
{
	if (loadOBJFile(modelFilename, modelData))
	{
		//TODO: gotta be a better way to do this. Don't make me make my own array 
		actors.reserve(numActorsToSpawn);
		for (int i = 0; i < numActorsToSpawn; i++)
		{
			Actor actor;
			actor.transform.r[3] = XMVectorSet(i, i, i, 1.f);
			actor.vertexBufferOffset = i * modelData.GetByteWidth();
			actors.push_back(actor);
		}

		UINT byteWidth = modelData.GetByteWidth();
		numVertices = (byteWidth * actors.size()) / sizeof(Vertex);
		dx->CreateVertexBuffer(byteWidth, modelData.verts.data(), this);

		size_t stride = sizeof(Vertex);
		//TODO: dislay debug rendering for box and sphere

		BoundingBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride); 
		BoundingSphere::CreateFromBoundingBox(boundingSphere, boundingBox);
	}
	else
	{
		OutputDebugString("Actor failed to load");
	}
}