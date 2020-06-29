#include "Actor.h"
#include "DXUtil.h"

Actor::Actor()
{
	transform = XMMatrixIdentity();
}

void Actor::CreateActor(const char* modelFilename, DXUtil* dx)
{
	if (loadOBJFile(modelFilename, modelData))
	{
		UINT byteWidth = modelData.GetByteWidth();
		dx->CreateVertexBuffer(modelData.GetByteWidth(), modelData.verts.data(), this); //Careful with the std::vector.data()

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