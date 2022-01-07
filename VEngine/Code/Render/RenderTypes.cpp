#include "RenderTypes.h"
#include "Material.h"
#include "Camera.h"
#include "Core.h"

void ShaderMatrices::Create()
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = activeCamera->GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(Material* material)
{
	//UV panning
	XMFLOAT2 offset = material->materialShaderData.uvOffset;
	offset.x += material->uvOffsetSpeed.x * Core::GetDeltaTime();
	offset.y += material->uvOffsetSpeed.y * Core::GetDeltaTime();
	material->materialShaderData.uvOffset = offset;
	XMVECTOR finalOffset = XMLoadFloat2(&offset);

	//Scaling isn't done, not much need for it.
	XMVECTOR scale = XMLoadFloat2(&material->materialShaderData.uvScale);

	//UV rotation
	float rotation = XMConvertToRadians(material->materialShaderData.uvRotation += material->uvRotationSpeed);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
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
