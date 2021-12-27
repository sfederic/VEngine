#include "RenderTypes.h"
#include "Material.h"
#include "Camera.h"

void ShaderMatrices::Create()
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = activeCamera->GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(MaterialShaderData* shaderData)
{
	XMVECTOR offset = XMLoadFloat2(&shaderData->uvOffset);
	XMVECTOR scale = XMLoadFloat2(&shaderData->uvScale);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f),
		shaderData->uvRotation, offset);
}

void ShaderMatrices::MakeModelViewProjectionMatrix()
{
	mvp = model * view * proj;
}

bool MeshDataProxy::CheckDuplicateVertices(Vertex& vert)
{
	auto pos = XMLoadFloat3(&vert.pos);

	const int size = vertices->size();
	for (int i = 0; i < size; i++)
	{
		XMVECTOR p = XMLoadFloat3(&vertices->at(i).pos);
		if (XMVector3Equal(p, pos))
		{
			return true;
		}
	}

	return false;
}

bool MeshDataProxy::CheckDuplicateIndices(MeshData::indexDataType index)
{
	int duplicateCounter = 0;

	for (int i = 0; i < indices->size(); i++)
	{
		if (index == indices->at(i))
		{
			duplicateCounter++;
			if (duplicateCounter >= 2)
			{
				return true;
			}
		}
	}

	return false;
}
