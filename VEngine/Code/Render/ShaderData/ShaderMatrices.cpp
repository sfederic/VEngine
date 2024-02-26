#include "vpch.h"
#include "ShaderMatrices.h"
#include "Render/Material.h"
#include "Render/MeshComponentFramePacket.h"
#include "Core/Camera.h"
#include "Core/Core.h"

void ShaderMatrices::Create()
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = Camera::GetActiveCamera().GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(MeshComponentFramePacket& mesh)
{
	//Don't animate while editing, fucks up starting uv offsets and rotations
	if (!Core::gameplayOn)
	{
		XMVECTOR finalOffset = XMLoadFloat2(&mesh.materialShaderData.uvOffset);
		XMVECTOR scale = XMLoadFloat2(&mesh.materialShaderData.uvScale);
		float rotation = XMConvertToRadians(mesh.materialShaderData.uvRotation);
		texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
		return;
	}

	//UV panning
	XMFLOAT2 offset = mesh.materialShaderData.uvOffset;
	offset.x += mesh.uvOffsetSpeed.x * Core::GetDeltaTime();
	offset.y += mesh.uvOffsetSpeed.y * Core::GetDeltaTime();
	mesh.materialShaderData.uvOffset = offset;
	XMVECTOR finalOffset = XMLoadFloat2(&offset);

	XMVECTOR scale = XMLoadFloat2(&mesh.materialShaderData.uvScale);

	//UV rotation
	float rotation = XMConvertToRadians(mesh.materialShaderData.uvRotation += mesh.uvRotationSpeed);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
}

void ShaderMatrices::MakeModelViewProjectionMatrix()
{
	mvp = model * view * proj;
}
