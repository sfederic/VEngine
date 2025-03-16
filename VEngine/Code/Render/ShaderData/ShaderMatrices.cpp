
#include "ShaderMatrices.h"
#include "Render/Material.h"
import Core.Camera;
import Core.Core;

void ShaderMatrices::Create()
{
	proj = Camera::GetActiveCamera().GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(Material& material)
{
	//Don't animate while editing, fucks up starting uv offsets and rotations
	if (!Core::gameplayOn)
	{
		XMVECTOR finalOffset = XMLoadFloat2(&material.GetMaterialShaderData().uvOffset);
		XMVECTOR scale = XMLoadFloat2(&material.GetMaterialShaderData().uvScale);
		float rotation = XMConvertToRadians(material.GetMaterialShaderData().uvRotation);
		texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
		return;
	}

	//UV panning
	XMFLOAT2 offset = material.GetMaterialShaderData().uvOffset;
	offset.x += material.uvOffsetSpeed.x * Core::GetDeltaTime();
	offset.y += material.uvOffsetSpeed.y * Core::GetDeltaTime();
	material.GetMaterialShaderData().uvOffset = offset;
	XMVECTOR finalOffset = XMLoadFloat2(&offset);

	XMVECTOR scale = XMLoadFloat2(&material.GetMaterialShaderData().uvScale);

	//UV rotation
	float rotation = XMConvertToRadians(material.GetMaterialShaderData().uvRotation += material.uvRotationSpeed);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f), rotation, finalOffset);
}

void ShaderMatrices::MakeModelViewProjectionMatrix()
{
	mvp = model * view * proj;
}
