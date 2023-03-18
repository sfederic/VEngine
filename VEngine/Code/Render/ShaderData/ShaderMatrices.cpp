#include "vpch.h"
#include "ShaderMatrices.h"
#include "Render/Material.h"
#include "Core/Camera.h"
#include "Core/Core.h"

void ShaderMatrices::Create()
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = activeCamera->GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(Material& material)
{
	//Don't animate while editing, fucks up starting uv offsets and rotations
	if (!Core::gameplayOn)
	{
		XMVECTOR finalOffset = XMLoadFloat2(&material.materialShaderData.uvOffset);
		XMVECTOR scale = XMLoadFloat2(&material.materialShaderData.uvScale);
		float rotation = XMConvertToRadians(material.materialShaderData.uvRotation);
		texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
		return;
	}

	//UV panning
	XMFLOAT2 offset = material.materialShaderData.uvOffset;
	offset.x += material.uvOffsetSpeed.x * Core::GetDeltaTime();
	offset.y += material.uvOffsetSpeed.y * Core::GetDeltaTime();
	material.materialShaderData.uvOffset = offset;
	XMVECTOR finalOffset = XMLoadFloat2(&offset);

	XMVECTOR scale = XMLoadFloat2(&material.materialShaderData.uvScale);

	//UV rotation
	float rotation = XMConvertToRadians(material.materialShaderData.uvRotation += material.uvRotationSpeed);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
}

void ShaderMatrices::MakeModelViewProjectionMatrix()
{
	mvp = model * view * proj;
}
